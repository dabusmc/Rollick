#include "Input/Keyboard.h"

#include "Library/kStandardLib.h"

namespace Kernel
{
	static const char s_ScancodeTable[] =
	{
		'\0', // 0x00
		'\0', // 0x01 (escape)
		'1',  // 0x02 (1)
		'2',  // 0x03 (2)
		'3',  // 0x04 (3)
		'4',  // 0x05 (4)
		'5',  // 0x06 (5)
		'6',  // 0x07 (6)
		'7',  // 0x08 (7)
		'8',  // 0x09 (8)
		'9',  // 0x0A (9)
		'0',  // 0x0B (0)
		'-',  // 0x0C (-)
		'=',  // 0x0D (=)
		'\0', // 0x0E (backspace)
		'\t', // 0x0F (tab)
		'q',  // 0x10 (Q)
	};

	void Keyboard::InterpretScancode(uint16_t scancode)
	{
		bool released = scancode & 0x80;
		if (released)
			return;

		bool extended = scancode & 0xFF00;

		uint8_t sc = scancode & 0xFF;

		if (sc >= sizeof(s_ScancodeTable))
			return;

		char character = s_ScancodeTable[sc];
		if (character == '\0')
			return;

		kprintf("%c", character);
	}
}
