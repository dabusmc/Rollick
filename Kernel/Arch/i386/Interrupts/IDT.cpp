#include "IDT.h"

extern "C"
{

void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

}

namespace Kernel
{
	namespace Interrupts
	{
		static IDTEntry s_IDT[256];
		static IDTPointer s_IDTPointer;

		static_assert(sizeof(IDTEntry) == 8);
		static_assert(sizeof(IDTPointer) == 6);

		using ISR = void (*)();
		static ISR ISRHandlers[32] =
		{
			isr0, isr1, isr2, isr3,
			isr4, isr5, isr6, isr7,
			isr8, isr9, isr10, isr11,
			isr12, isr13, isr14, isr15,
			isr16, isr17, isr18, isr19,
			isr20, isr21, isr22, isr23,
			isr24, isr25, isr26, isr27,
			isr28, isr29, isr30, isr31
		};

		void InitializeIDT()
		{
			for (uint32_t i = 0; i < 32; i++)
			{
				uint32_t handlerAddress = reinterpret_cast<uint32_t>(ISRHandlers[i]);

				s_IDT[i].OffsetLow = handlerAddress & 0xFFFF;
				s_IDT[i].Selector = 0x08;
				s_IDT[i].Zero = 0;
				s_IDT[i].TypeAttributes = 0x8E;
				s_IDT[i].OffsetHigh = (handlerAddress >> 16) & 0xFFFF;
			}

			s_IDTPointer.Limit = sizeof(s_IDT) - 1;
			s_IDTPointer.Base = reinterpret_cast<uint32_t>(&s_IDT[0]);

			asm volatile("lidt %0" : : "m"(s_IDTPointer));
		}
	}
}
