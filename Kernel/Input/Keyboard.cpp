#include "Input/Keyboard.h"

#include "Library/kStandardLib.h"

#include "Kernel.h"

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

	static constexpr char s_ScancodeTableShifted[0x59] =
	{
		// 0x00 - 0x0F
		'\0', '\x1B', '!', '@',
		'#', '$', '%', '^',
		'&', '*', '(', ')',
		'_', '+', '\b', '\t',

		// 0x10 - 0x1F
		'Q', 'W', 'E', 'R',
		'T', 'Y', 'U', 'I',
		'O', 'P', '{', '}',
		'\n', '\0', 'A', 'S',

		// 0x20 - 0x2F
		'D', 'F', 'G', 'H',
		'J', 'K', 'L', ':',
		'"', '~', '\0', '|',
		'Z', 'X', 'C', 'V',

		// 0x30 - 0x3F
		'B', 'N', 'M', '<',
		'>', '?', '\0', '*',
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

	static bool IsLetter(KeyCode key)
	{
		uint16_t code = static_cast<uint16_t>(key);

		return (code >= 0x10 && code <= 0x19) ||
			   (code >= 0x1E && code <= 0x26) ||
			   (code >= 0x2C && code <= 0x32);
	}

	char Keyboard::TranslateKey(KeyCode keycode) const
	{
		uint16_t sc = static_cast<uint16_t>(keycode);
		if (sc >= sizeof(s_ScancodeTable))
		{
			return '\0';
		}

		bool shifted = m_LeftShiftPressed || m_RightShiftPressed;

		if (m_CapsLockEnabled && IsLetter(keycode))
		{
			shifted = !shifted;
		}

		return shifted ? s_ScancodeTableShifted[sc] : s_ScancodeTable[sc];
	}

	void Keyboard::ProcessKeyEvent(const KeyEvent& event)
	{
		bool pressed = event.State == KeyState::Pressed;

		switch (event.Key)
		{
		case KeyCode::LeftShift:
			m_LeftShiftPressed = pressed;
			return;
		case KeyCode::RightShift:
			m_RightShiftPressed = pressed;
			return;
		case KeyCode::CapsLock:
			if (pressed && !m_CapsLockPressed)
			{
				m_CapsLockEnabled = !m_CapsLockEnabled;
			}
			m_CapsLockPressed = pressed;
			return;
		default:
			break;
		}

		if (!pressed)
		{
			return;
		}

		char key = TranslateKey(event.Key);
		if (key != '\0')
		{
			kprintf("%c", key);
		}
	}
}
