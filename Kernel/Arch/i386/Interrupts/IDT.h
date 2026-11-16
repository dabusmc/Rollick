#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Interrupts
	{
		struct IDTEntry
		{
			uint16_t OffsetLow;
			uint16_t Selector;
			uint8_t Zero;
			uint8_t TypeAttributes;
			uint16_t OffsetHigh;
		} __attribute__((packed));

		struct IDTPointer
		{
			uint16_t Limit;
			uint32_t Base;
		} __attribute__((packed));

		void InitializeIDT();
	}
}