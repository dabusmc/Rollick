#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Memory
	{
		namespace VMM
		{
			bool MapPage(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t flags);
			bool UnmapPage(uint32_t virtualAddress);
			bool GetPhysicalAddress(uint32_t virtualAddress, uint32_t& physicalAddress);
		}
	}
}