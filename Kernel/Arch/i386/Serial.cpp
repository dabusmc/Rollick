#include "Arch/i386/Serial.h"

#include "Library/kStandardLib.h"

#define COM1 0x3F8

namespace Kernel
{
	Serial::Serial()
	{
		// Disable interrupts
		outb(0x00, COM1 + 1);

		// Enable divisor latch
		outb(0x80, COM1 + 3);

		// Set baud rate to 115200
		outb(0x01, COM1);
		outb(0x00, COM1 + 1);

		// 8 bits, no parity, one stop bit
		outb(0x03, COM1 + 3);

		// Enable FIFO, clear buffers, 14-byte threshold
		outb(0xC7, COM1 + 2);

		// Enable DTR, RTS and OUT2
		outb(0x0B, COM1 + 4);
	}

	void Serial::PutChar(char c)
	{
		// Wait until the transmit buffer is empty
		while (!(inb(COM1 + 5) & 0x20))
		{
		}

		// Send the character
		outb(c, COM1);
	}

	void Serial::Write(const char* data, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (data[i] == '\n')
			{
				PutChar('\r');
			}

			PutChar(data[i]);
		}
	}
}