#include "Library/kStandardLib.h"

#include "Arch/i386/Interrupts/IDT.h"

extern "C" void kernelMain(void)
{
	Kernel::Interrupts::InitializeIDT();

	volatile int numerator = 10;
	volatile int denominator = 0;

	int result;

	asm volatile(
		"movl %1, %%eax\n"
		"cltd\n"
		"idivl %2\n"
		: "=a"(result)
		: "r"(numerator), "r"(denominator)
		: "edx"
	);

	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %s%c\n", "Rollick", "OS", '!');
	kprintf("Progress: %i%%\n", 100);
}