#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Memory
	{
		namespace Paging
		{
			constexpr uint32_t PageSize = 4096;

			enum PageFlags : uint32_t
			{
				Present      = 1 << 0,
				Writable     = 1 << 1,
				User         = 1 << 2,
				WriteThrough = 1 << 3,
				CacheDisable = 1 << 4,
				Accessed     = 1 << 5,
				Dirty        = 1 << 6,
				PageSize4MB  = 1 << 7,
				Global       = 1 << 8
			};

			struct PageTableEntry
			{
				uint32_t Value;
			};

			struct PageDirectoryEntry
			{
				uint32_t Value;
			};
		}
	}
}