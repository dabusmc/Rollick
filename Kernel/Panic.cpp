#include "Panic.h"

#include "Arch/i386/Interrupts/InterruptFrame.h"

#include "Library/kStandardLib.h"

namespace Kernel
{
	static void PrintRegister(const char* name, uint32_t value)
	{
		kprintf("%s: 0x%x\n", name, value);
	}

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

	[[noreturn]] void Panic(const char* message, const Interrupts::InterruptFrame* frame)
	{
		asm volatile("cli" ::: "memory");

		clrscr();

		kprintf("*** KERNEL PANIC ***\n");
		kprintf("Reason: %s\n\n", message);

		kprintf("Exception: %d\n", frame->InterruptNumber);
		PrintRegister("Error Code", frame->ErrorCode);

		kprintf("\n--- CPU STATE ---\n");

		PrintRegister("EIP", frame->EIP);
		PrintRegister("EFLAGS", frame->EFlags);
		PrintRegister("EAX", frame->EAX);
		PrintRegister("EBX", frame->EBX);
		PrintRegister("ECX", frame->ECX);
		PrintRegister("EDX", frame->EDX);
		PrintRegister("ESI", frame->ESI);
		PrintRegister("EDI", frame->EDI);
		PrintRegister("EBP", frame->EBP);
		PrintRegister("ESP", frame->ESP);
		PrintRegister("CS", frame->CS);
		PrintRegister("DS", frame->DS);
		PrintRegister("ES", frame->ES);
		PrintRegister("FS", frame->FS);
		PrintRegister("GS", frame->GS);

		while (true)
		{
			asm volatile("hlt");
		}
	}
}