#include "Panic.h"
#include "Library/kStandardLib.h"

extern "C" void kernelMain(void)
{
	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %c %s\n", "Rollick", '!', "OS");
	kprintf("Progress: 100%%\n");

	KERNEL_PANIC("Testing kernel panic");
}