#include "Input/Keyboard.h"

#include "Library/kStandardLib.h"

namespace Kernel
{

	static constexpr char s_ScancodeTable[0x59] =
	{
		// 0x00 - 0x0F
		'\0', '\x1B', '1', '2',
		'3', '4', '5', '6',
		'7', '8', '9', '0',
		'-', '=', '\b', '\t',

		// 0x10 - 0x1F
		'q', 'w', 'e', 'r',
		't', 'y', 'u', 'i',
		'o', 'p', '[', ']',
		'\n', '\0', 'a', 's',

		// 0x20 - 0x2F
		'd', 'f', 'g', 'h',
		'j', 'k', 'l', ';',
		'\'', '`', '\0', '\\',
		'z', 'x', 'c', 'v',

		// 0x30 - 0x3F
		'b', 'n', 'm', ',',
		'.', '/', '\0', '*',
		'\0', ' ', '\0', '\0',
		'\0', '\0', '\0', '\0',

		// 0x40 - 0x4F
		'\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '7',
		'8', '9', '-', '4',
		'5', '6', '+', '1',

		// 0x50 - 0x58
		'2', '3', '0', '.',
		'\0', '\0', '\\', '\0',
		'\0'
	};


	char Keyboard::TranslateKey(KeyCode keycode) const
	{
		uint16_t sc = static_cast<uint16_t>(keycode);
		if (sc >= sizeof(s_ScancodeTable))
		{
			return '\0';
		}

		return s_ScancodeTable[sc];
	}

	void Keyboard::InterpretScancode(uint16_t scancode)
	{
		bool released = scancode & 0x80;
		if (released)
		{
			return;
		}

		char key = TranslateKey(static_cast<KeyCode>(scancode));
		if (key == '\0')
		{
			return;
		}

		kprintf("%c", key);
	}
}
