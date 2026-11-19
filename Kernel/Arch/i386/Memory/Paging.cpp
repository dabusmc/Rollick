#include "Arch/i386/Memory/Paging.h"

#include "Memory/PhysicalMemoryManager.h"

#include "Kernel.h"

namespace Kernel
{
	namespace Memory
	{
	    namespace Paging
	    {
	        namespace
	        {
	            constexpr uint32_t AddressMask = 0xFFFFF000;
	            constexpr uint32_t FlagsMask = 0x00000FFF;

	            void InvalidatePage(uint32_t address)
	            {
	                asm volatile("invlpg (%0)" :: "r"(address) : "memory");
	            }
	        }

	        bool MapPage(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t flags)
	        {
	            // Both addresses must be page-aligned.
	            if ((virtualAddress % PageSize) != 0 || (physicalAddress % PageSize) != 0)
	            {
	                return false;
	            }

	            uint32_t directoryIndex = GetPageDirectoryIndex(virtualAddress);
	            uint32_t tableIndex = GetPageTableIndex(virtualAddress);

	            // Never overwrite the recursive page directory entry.
	            if (directoryIndex == 1023)
	            {
		            return false;
	            }

	            auto* pageDirectory = GetPageDirectory();
	            uint32_t& directoryEntry = pageDirectory[directoryIndex];

	            // Create a page table if one doesn't exist.
	            if (!(directoryEntry & Present))
	            {
	                uint64_t tablePhysicalAddress = g_GlobalObjects.Memory_PMM.AllocatePage();

	                if (tablePhysicalAddress == 0)
	                {
		                return false;
	                }

	                directoryEntry = static_cast<uint32_t>(tablePhysicalAddress) | Present | Writable | (flags & User);

	                // Make the newly accessible recursive page-table mapping valid.
	                uint32_t tableVirtualAddress = RecursivePageTableAddress + (directoryIndex * PageSize);
	                InvalidatePage(tableVirtualAddress);
	                auto* newPageTable = reinterpret_cast<uint32_t*>(tableVirtualAddress);

	                // Clear all 1024 entries.
	                for (size_t i = 0; i < 1024; ++i)
	                {
	                    newPageTable[i] = 0;
	                }
	            }
	            else
	            {
	                // We currently only support ordinary 4 KiB page tables.
	                if (directoryEntry & PageSize4MB)
	                {
		                return false;
	                }

	                // A user-accessible page also requires a user-accessible PDE.
	                if (flags & User)
	                {
	                    directoryEntry |= User;
	                }
	            }

	            auto* pageTable = GetPageTable(virtualAddress);
	            if (pageTable == nullptr)
	            {
		            return false;
	            }

	            uint32_t& pageEntry = pageTable[tableIndex];
	            if (pageEntry & Present)
	            {
		            return false;
	            }

	            pageEntry = (physicalAddress & AddressMask) | ((flags | Present) & FlagsMask);
	            InvalidatePage(virtualAddress);
	            return true;
	        }

	        bool UnmapPage(uint32_t virtualAddress)
	        {
	            if ((virtualAddress % PageSize) != 0)
	            {
		            return false;
	            }

	            uint32_t directoryIndex = GetPageDirectoryIndex(virtualAddress);
	            if (directoryIndex == 1023)
	            {
		            return false;
	            }

	            auto* pageTable = GetPageTable(virtualAddress);
	            if (pageTable == nullptr)
	            {
		            return false;
	            }

	            uint32_t tableIndex = GetPageTableIndex(virtualAddress);
	            uint32_t& pageEntry = pageTable[tableIndex];
	            if (!(pageEntry & Present))
	            {
		            return false;
	            }

	            pageEntry = 0;
	            InvalidatePage(virtualAddress);
	            return true;
	        }

	        bool GetPhysicalAddress(uint32_t virtualAddress, uint32_t& physicalAddress)
	        {
	            uint32_t directoryIndex = GetPageDirectoryIndex(virtualAddress);

	            if (directoryIndex == 1023)
	            {
		            return false;
	            }

	            auto* pageDirectory = GetPageDirectory();
	            if (!(pageDirectory[directoryIndex] & Present))
	            {
		            return false;
	            }

	            // We don't currently support 4 MiB pages.
	            if (pageDirectory[directoryIndex] & PageSize4MB)
	            {
		            return false;
	            }

	            auto* pageTable = GetPageTable(virtualAddress);
	            if (pageTable == nullptr)
	            {
		            return false;
	            }

	            uint32_t tableIndex = GetPageTableIndex(virtualAddress);
	            uint32_t pageEntry = pageTable[tableIndex];
	            if (!(pageEntry & Present))
	            {
		            return false;
	            }

	            physicalAddress = (pageEntry & AddressMask) | (virtualAddress & (PageSize - 1));
	            return true;
	        }
	    }
	}
}