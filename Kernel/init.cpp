#include "Panic.h"
#include "Library/kStandardLib.h"

__attribute__((noinline)) void TestStackSmashing()
{
	volatile char buffer[8];

	for (int i = 0; i < 32; i++)
	{
		buffer[i] = 'A';
	}
}

extern "C" void kernelMain(void)
{
	TestStackSmashing();

	kprintf("Hello, Rollick!\n");
	kprintf("Character: %c\n", 'A');
	kprintf("String: %s\n", "Hello World");
	kprintf("Mixed: %s %c %s\n", "Rollick", '!', "OS");
	kprintf("Progress: %i%%\n", 100);
}