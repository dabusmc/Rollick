#include "Arch/i386/Interrupts/PIC.h"

#include "Library/kStandardLib.h"

namespace Kernel
{
	namespace Interrupts
	{
		namespace
		{
			constexpr uint16_t PIC1_COMMAND = 0x20;
			constexpr uint16_t PIC1_DATA    = 0x21;
			constexpr uint16_t PIC2_COMMAND = 0xA0;
			constexpr uint16_t PIC2_DATA    = 0xA1;
		}

		void InitializePIC()
		{
			// Disable interrupts during configuration
			asm volatile("cli" ::: "memory");

			// Begin initialisation sequence
			outb_p(0x11, PIC1_COMMAND);
			outb_p(0x11, PIC2_COMMAND);

			// Set interrupt vector offsets
			outb_p(0x20, PIC1_DATA);
			outb_p(0x28, PIC2_DATA);

			// Configure cascading
			outb_p(0x04, PIC1_DATA);
			outb_p(0x02, PIC2_DATA);

			// Set 8086 mode
			outb_p(0x01, PIC1_DATA);
			outb_p(0x01, PIC2_DATA);

			// Mask all IRQs for now
			outb_p(0xFF, PIC1_DATA);
			outb_p(0xFF, PIC2_DATA);
		}

		void SendEOI(uint8_t interruptNumber)
		{
			// Acknowledge the slave PIC first, if applicable.
			if (interruptNumber >= 40)
			{
				outb_p(0x20, PIC2_COMMAND);
			}

			// Always acknowledge the master PIC.
			outb_p(0x20, PIC1_COMMAND);
		}

		void SetIRQMask(uint8_t irq, bool masked)
		{
			if (irq >= 16)
				return;

			uint16_t port = irq < 8 ? PIC1_DATA : PIC2_DATA;
			uint8_t bit = irq < 8 ? irq : irq - 8;

			uint8_t mask = inb(port);

			if (masked)
			{
				mask |= (1 << bit);
			}
			else
			{
				mask &= ~(1 << bit);
			}

			outb_p(mask, port);

			// Slave PIC interrupts require the master cascade (IRQ2).
			if (irq >= 8 && !masked)
			{
				SetIRQMask(2, false);
			}
		}
	}
}