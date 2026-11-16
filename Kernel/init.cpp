#include "Library/kStandardLib.h"

#include "Arch/i386/Interrupts/IDT.h"
#include "Arch/i386/Interrupts/PIC.h"
#include "Arch/i386/Interrupts/PIT.h"

extern "C" void kernelMain(void)
{
	Kernel::Interrupts::InitializeIDT();
	Kernel::Interrupts::InitializePIC();

	Kernel::Interrupts::InitializePIT(100);
	Kernel::Interrupts::SetIRQMask(0, false);

	asm volatile("sti" ::: "memory");

	uint32_t start = Kernel::Interrupts::GetPITTicks();
	while (Kernel::Interrupts::GetPITTicks() - start < 100)
	{
		// Wait for approximately one second.
	}

	kprintf("PIT ticks: %d\n", Kernel::Interrupts::GetPITTicks());

	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %s%c\n", "Rollick", "OS", '!');
	kprintf("Progress: %i%%\n", 100);
}
