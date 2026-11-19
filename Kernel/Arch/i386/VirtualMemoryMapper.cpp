#include "Memory/VirtualMemoryManager.h"

#include "Arch/i386/Memory/Paging.h"

namespace Kernel
{
	namespace Memory
	{
		namespace VMM
		{
			bool MapPage(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t flags)
			{
				return Paging::MapPage(virtualAddress, physicalAddress, flags);
			}

			bool UnmapPage(uint32_t virtualAddress)
			{
				return Paging::UnmapPage(virtualAddress);
			}

			bool GetPhysicalAddress(uint32_t virtualAddress, uint32_t& physicalAddress)
			{
				return Paging::GetPhysicalAddress(virtualAddress, physicalAddress);
			}
		}
	}
}