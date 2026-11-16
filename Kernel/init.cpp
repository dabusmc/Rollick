#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Terminal.h"

#include "Library/kStandardLib.h"

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void kernelMain(void)
{
	/* Initialize terminal interface */
	terminalInitialize();

	/* Newline support is left as an exercise. */
	terminalWriteString("Hello, kernel World 1!\n");
	terminalWriteString("Hello, kernel World 2!\n");
	terminalWriteString("Hello, kernel World 3!\n");
	terminalWriteString("Hello, kernel World 4!\n");
	terminalWriteString("Hello, kernel World 5!\n");
	terminalWriteString("Hello, kernel World 6!\n");
	terminalWriteString("Hello, kernel World 7!\n");
	terminalWriteString("Hello, kernel World 8!\n");
	terminalWriteString("Hello, kernel World 9!\n");
	terminalWriteString("Hello, kernel World 10!\n");
	terminalWriteString("Hello, kernel World 11!\n");
	terminalWriteString("Hello, kernel World 12!\n");
	terminalWriteString("Hello, kernel World 13!\n");
	terminalWriteString("Hello, kernel World 14!\n");
	terminalWriteString("Hello, kernel World 15!\n");
	terminalWriteString("Hello, kernel World 16!\n");
	terminalWriteString("Hello, kernel World 17!\n");
	terminalWriteString("Hello, kernel World 18!\n");
	terminalWriteString("Hello, kernel World 19!\n");
	terminalWriteString("Hello, kernel World 20!\n");
	terminalWriteString("Hello, kernel World 21!\n");
	terminalWriteString("Hello, kernel World 22!\n");
	terminalWriteString("Hello, kernel World 23!\n");
	terminalWriteString("Hello, kernel World 24!\n");
	terminalWriteString("Hello, kernel World 25!\n");
	terminalWriteString("Hello, kernel World 26!\n");
	terminalWriteString("Hello, kernel World 27!\n");
	terminalWriteString("Hello, kernel World 28!\n");
	terminalWriteString("Hello, kernel World 29!\n");

	kprintf("Hello, %s\n", "kprintf!");
}