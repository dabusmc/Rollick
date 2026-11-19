#pragma once

#include "Arch/i386/Memory/PageTable.h"

#include <stdint.h>

extern "C"
{

extern uint32_t PageDirectory[1024];

}

namespace Kernel
{
	namespace Memory
	{
		namespace Paging
		{
			constexpr uint32_t RecursivePageTableAddress = 0xFFC00000;
			constexpr uint32_t RecursivePageDirectoryAddress = 0xFFFFF000;

			constexpr uint32_t GetPageDirectoryIndex(uint32_t address)
			{
				return (address >> 22) & 0x3FF;
			}

			constexpr uint32_t GetPageTableIndex(uint32_t address)
			{
				return (address >> 12) & 0x3FF;
			}

			inline uint32_t* GetPageDirectory()
			{
				return reinterpret_cast<uint32_t*>(RecursivePageDirectoryAddress);
			}

			inline uint32_t* GetPageTable(uint32_t virtualAddress)
			{
				uint32_t directoryIndex = GetPageDirectoryIndex(virtualAddress);

				uint32_t* pageDirectory = GetPageDirectory();
				if (!(pageDirectory[directoryIndex] & Present))
				{
					return nullptr;
				}

				uint32_t pageTableAddress = RecursivePageTableAddress + (directoryIndex * PageSize);
				return reinterpret_cast<uint32_t*>(pageTableAddress);
			}

			inline uint32_t* GetPageTableEntry(uint32_t virtualAddress)
			{
				uint32_t* pageTable = GetPageTable(virtualAddress);
				if (pageTable == nullptr)
				{
					return nullptr;
				}

				return &pageTable[GetPageTableIndex(virtualAddress)];
			}

			bool MapPage(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t flags = Present | Writable);
			bool UnmapPage(uint32_t virtualAddress);
			bool GetPhysicalAddress(uint32_t virtualAddress, uint32_t& physicalAddress);
		}
	}
}