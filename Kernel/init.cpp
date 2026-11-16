#include "Library/kStandardLib.h"

#include "Arch/i386/Interrupts/IDT.h"

extern "C" void kernelMain(void)
{
	Kernel::Interrupts::InitializeIDT();

	asm volatile("int $0");

	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %s%c\n", "Rollick", "OS", '!');
	kprintf("Progress: %i%%\n", 100);
}