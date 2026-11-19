#include "Memory/Heap.h"

#include "Memory/PhysicalMemoryManager.h"
#include "Memory/VirtualMemoryManager.h"

#include "Arch/i386/Memory/PageTable.h"

#include "Kernel.h"

#include <stdint.h>

namespace Kernel
{
    namespace Memory
    {
        constexpr uint32_t c_HeapStart = 0xD0000000;
        constexpr uint32_t c_HeapLimit = 0xD1000000;

        constexpr size_t c_Alignment = 8;

        constexpr uint32_t c_BlockMagic = 0xB10CDA7A;

        Heap::Heap()
            : m_HeapEnd(c_HeapStart)
        {

        }

        bool Heap::Initialize()
        {
            if (m_Initialized)
            {
                return false;
            }

            m_HeapEnd = c_HeapStart;
            m_FirstBlock = nullptr;
            m_LastBlock = nullptr;

            if (!ExpandHeap())
            {
                return false;
            }

            m_Initialized = true;
            return true;
        }

        void* Heap::Allocate(size_t size)
        {
            if (!m_Initialized || size == 0)
            {
                return nullptr;
            }

            // Align allocations to eight bytes.
            if (size > static_cast<size_t>(-1) - (c_Alignment - 1))
            {
                return nullptr;
            }

            size = (size + c_Alignment - 1) & ~(c_Alignment - 1);

            while (true)
            {
                for (Block* block = m_FirstBlock; block != nullptr; block = block->Next)
                {
                    if (!block->Free || block->Size < size)
                    {
                        continue;
                    }

                    SplitBlock(block, size);
                    block->Free = 0;
                    return block + 1;
                }

                // No suitable block exists. Grow the heap and try again.
                if (!ExpandHeap())
                {
                    return nullptr;
                }
            }
        }

        void Heap::Free(void* pointer)
        {
            if (!m_Initialized || pointer == nullptr)
            {
                return;
            }

            uintptr_t address = reinterpret_cast<uintptr_t>(pointer);
            if (address < c_HeapStart + sizeof(Block) || address >= m_HeapEnd)
            {
                return;
            }

            auto* block = reinterpret_cast<Block*>(pointer) - 1;
            if (block->Magic != c_BlockMagic || block->Free)
            {
                return;
            }

            block->Free = 1;

            // Merge into the previous block if possible.
            if (block->Previous != nullptr && block->Previous->Free)
            {
                block = block->Previous;
                MergeWithNext(block);
            }

            // Merge with the next block if possible.
            MergeWithNext(block);
        }

        void Heap::ZeroPage(uint32_t address)
        {
            volatile uint32_t* memory = reinterpret_cast<volatile uint32_t*>(address);
            for (size_t i = 0; i < c_PageSize / sizeof(uint32_t); ++i)
            {
                memory[i] = 0;
            }
        }

        bool Heap::ExpandHeap()
        {
            if (m_HeapEnd > c_HeapLimit - c_PageSize)
            {
                return false;
            }

            uint64_t physicalAddress = g_GlobalObjects.Memory_PMM.AllocatePage();
            if (physicalAddress == 0)
            {
                return false;
            }

            uint32_t virtualAddress = m_HeapEnd;

            bool mapped = VMM::MapPage( virtualAddress, static_cast<uint32_t>(physicalAddress), Paging::Present | Paging::Writable);
            if (!mapped)
            {
                g_GlobalObjects.Memory_PMM.FreePage(physicalAddress);

                return false;
            }

            m_HeapEnd += c_PageSize;

            ZeroPage(virtualAddress);

            // Extend the previous block if it is free and contiguous.
            if (m_LastBlock != nullptr && m_LastBlock->Free)
            {
                uintptr_t blockEnd = reinterpret_cast<uintptr_t>(m_LastBlock + 1) + m_LastBlock->Size;
                if (blockEnd == virtualAddress)
                {
                    m_LastBlock->Size += c_PageSize;
                    return true;
                }
            }

            // Otherwise create a new free block.
            auto* block = reinterpret_cast<Block*>(virtualAddress);

            block->Size = c_PageSize - sizeof(Block);
            block->Previous = m_LastBlock;
            block->Next = nullptr;
            block->Magic = c_BlockMagic;
            block->Free = 1;

            if (m_LastBlock != nullptr)
            {
                m_LastBlock->Next = block;
            }
            else
            {
                m_FirstBlock = block;
            }

            m_LastBlock = block;

            return true;
        }

        void Heap::SplitBlock(Block* block, size_t size)
        {
            if (block->Size < size + sizeof(Block) + c_Alignment)
            {
                return;
            }

            auto* split = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(block + 1) + size);

            split->Size = block->Size - size - sizeof(Block);
            split->Previous = block;
            split->Next = block->Next;
            split->Magic = c_BlockMagic;
            split->Free = 1;

            if (split->Next != nullptr)
            {
                split->Next->Previous = split;
            }
            else
            {
                m_LastBlock = split;
            }

            block->Next = split;
            block->Size = size;
        }

        void Heap::MergeWithNext(Block* block)
        {
            Block* next = block->Next;
            if (next == nullptr || !next->Free)
            {
                return;
            }

            block->Size += sizeof(Block) + next->Size;
            block->Next = next->Next;

            if (block->Next != nullptr)
            {
                block->Next->Previous = block;
            }
            else
            {
                m_LastBlock = block;
            }
        }
    }
}