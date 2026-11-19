#pragma once

#include "Memory/Memory.h"

#include <stdint.h>
#include <stddef.h>

namespace Kernel
{
	namespace Memory
	{
		constexpr uint64_t c_PageSize = 4096;
		constexpr uint64_t c_MaxPhysicalAddress = 0x08000000; // 128 MiB

		constexpr size_t c_PageCount = c_MaxPhysicalAddress / c_PageSize;
		constexpr size_t c_BitmapSize = (c_PageCount + 7) / 8;

		class PhysicalMemoryManager
		{
		public:
			PhysicalMemoryManager();
			void Initialize(const MemoryRegion* regions, size_t regionCount);

			size_t GetFreePageCount() const { return m_FreePageCount; }
			void ReserveRange(uint64_t base, uint64_t length);

			uint64_t AllocatePage();
			bool FreePage(uint64_t address);

		private:
			bool IsPageAllocated(size_t page);
			void MarkPageFree(size_t page);
			void MarkPageAllocated(size_t page);

		private:
			uint8_t m_Bitmap[c_BitmapSize];
			size_t m_FreePageCount = 0;
			bool m_Initialised = false;
		};
	}
}