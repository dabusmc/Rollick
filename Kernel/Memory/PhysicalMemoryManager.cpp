#include "Memory/PhysicalMemoryManager.h"

namespace Kernel
{
	namespace Memory
	{
		PhysicalMemoryManager::PhysicalMemoryManager()
		{
		}

		void PhysicalMemoryManager::Initialize(const MemoryRegion* regions, size_t regionCount)
		{
			// Initially, consider every page unavailable.
			for (size_t i = 0; i < c_BitmapSize; ++i)
			{
				m_Bitmap[i] = 0xFF;
			}

			m_FreePageCount = 0;
			m_Initialised = false;

			// Mark usable memory regions as free.
			for (size_t i = 0; i < regionCount; ++i)
			{
				const auto& region = regions[i];

				if (region.Type != MemoryRegionType::Available)
				{
					continue;
				}

				if (region.BaseAddress >= c_MaxPhysicalAddress)
				{
					continue;
				}

				uint64_t end = region.Size > c_MaxPhysicalAddress - region.BaseAddress ? c_MaxPhysicalAddress : region.BaseAddress + region.Size;

				size_t firstPage = static_cast<size_t>(region.BaseAddress / c_PageSize + (region.BaseAddress % c_PageSize != 0));
				size_t lastPage = static_cast<size_t>(end / c_PageSize);

				for (size_t page = firstPage; page < lastPage; ++page)
				{
					MarkPageFree(page);
				}
			}

			m_Initialised = true;
		}

		void PhysicalMemoryManager::ReserveRange(uint64_t base, uint64_t length)
		{
			if (!m_Initialised || length == 0 || base >= c_MaxPhysicalAddress)
			{
				return;
			}

			uint64_t end = length > c_MaxPhysicalAddress - base ? c_MaxPhysicalAddress : base + length;

			// Reserve every page touched by this range.
			size_t firstPage = static_cast<size_t>(base / c_PageSize);
			size_t lastPage = static_cast<size_t>(end / c_PageSize + (end % c_PageSize != 0));

			for (size_t page = firstPage; page < lastPage; ++page)
			{
				MarkPageAllocated(page);
			}
		}

		uint64_t PhysicalMemoryManager::AllocatePage()
		{
			if (!m_Initialised)
			{
				return 0;
			}

			if (m_FreePageCount == 0)
			{
				return 0;
			}

			for (size_t page = 0; page < c_PageCount; ++page)
			{
				if (!IsPageAllocated(page))
				{
					MarkPageAllocated(page);
					return static_cast<uint64_t>(page) * c_PageSize;
				}
			}

			return 0;
		}

		bool PhysicalMemoryManager::FreePage(uint64_t address)
		{
			if (!m_Initialised)
			{
				return false;
			}

			if (address >= c_MaxPhysicalAddress || address % c_PageSize != 0)
			{
				return false;
			}

			size_t page = static_cast<size_t>(address / c_PageSize);
			if (IsPageAllocated(page))
			{
				MarkPageFree(page);
				return true;
			}

			return false;
		}

		bool PhysicalMemoryManager::IsPageAllocated(size_t page)
		{
			size_t byteIndex = page / 8;
			uint8_t bitMask = static_cast<uint8_t>(1u << (page % 8));

			return (m_Bitmap[byteIndex] & bitMask) != 0;
		}

		void PhysicalMemoryManager::MarkPageFree(size_t page)
		{
			if (IsPageAllocated(page))
			{
				size_t byteIndex = page / 8;
				uint8_t bitMask = static_cast<uint8_t>(1u << (page % 8));

				m_Bitmap[byteIndex] &= static_cast<uint8_t>(~bitMask);
				++m_FreePageCount;
			}
		}

		void PhysicalMemoryManager::MarkPageAllocated(size_t page)
		{
			if (!IsPageAllocated(page))
			{
				size_t byteIndex = page / 8;
				uint8_t bitMask = static_cast<uint8_t>(1u << (page % 8));

				m_Bitmap[byteIndex] |= bitMask;
				--m_FreePageCount;
			}
		}
	}
}