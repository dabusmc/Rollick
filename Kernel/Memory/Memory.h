#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Memory
	{
		enum class MemoryRegionType
		{
			Available,
			Reserved
		};

		struct MemoryRegion
		{
			uint64_t BaseAddress;
			uint64_t Size;
			MemoryRegionType Type;
		};
	}
}