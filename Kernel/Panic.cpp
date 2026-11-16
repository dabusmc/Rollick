#include "Panic.h"

#include "Library/kStandardLib.h"

namespace Kernel
{
	[[noreturn]] void Panic(const char* message, const char* file, int line)
	{
		// Disable interrupts
		asm volatile("cli" ::: "memory");

		// Print Message
		clrscr();
		kprintf("*** KERNEL PANIC ***\n");
		kprintf("Reason: %s\n", message);
		kprintf("Location: %s:%d\n", file, line);

		// Halt permanently
		while (true)
		{
			asm volatile("hlt");
		}
	}
}