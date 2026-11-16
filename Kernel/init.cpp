#include "Library/kStandardLib.h"

extern "C" void kernelMain(void)
{
	kprintf("Hello, %s\n", "kprintf!");
}