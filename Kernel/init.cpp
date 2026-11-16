#include "Library/kStandardLib.h"

extern "C" void kernelMain(void)
{
	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %s%c\n", "Rollick", "OS", '!');
	kprintf("Progress: %i%%\n", 100);
}