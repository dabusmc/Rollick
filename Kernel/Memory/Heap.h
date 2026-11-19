#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Kernel
{
	namespace Memory
	{
		bool Initialise();

		void* Allocate(size_t size);
		void Free(void* pointer);

		class Heap
		{
		public:
			Heap();
			bool Initialize();

			void* Allocate(size_t size);
			void Free(void* pointer);

		private:
			struct alignas(8) Block
			{
				size_t Size;
				Block* Previous;
				Block* Next;
				uint32_t Magic;
				uint32_t Free;
			};

		private:
			void ZeroPage(uint32_t address);
			bool ExpandHeap();
			void SplitBlock(Block* block, size_t size);
			void MergeWithNext(Block* block);

		private:
			uint32_t m_HeapEnd;
			bool m_Initialized = false;
			Block* m_FirstBlock = nullptr;
			Block* m_LastBlock = nullptr;
		};
	}
}