#include "IDT.h"

extern "C" void default_interrupt_handler();

namespace Kernel
{
	namespace Interrupts
	{
		static IDTEntry s_IDT[256];
		static IDTPointer s_IDTPointer;

		static_assert(sizeof(IDTEntry) == 8);
		static_assert(sizeof(IDTPointer) == 6);

		void InitializeIDT()
		{
			const uint32_t handlerAddress = reinterpret_cast<uint32_t>(&default_interrupt_handler);
			for (uint16_t i = 0; i < 256; i++)
			{
				s_IDT[i].OffsetLow = handlerAddress & 0xFFFF;
				s_IDT[i].Selector = 0x08;
				s_IDT[i].Zero = 0;
				s_IDT[i].TypeAttributes = 0b10001110;
				s_IDT[i].OffsetHigh = (handlerAddress >> 16) & 0xFFFF;
			}

			s_IDTPointer.Limit = sizeof(s_IDT) - 1;
			s_IDTPointer.Base = reinterpret_cast<uint32_t>(&s_IDT[0]);

			asm volatile("lidt %0" : : "m"(s_IDTPointer));
		}
	}
}
