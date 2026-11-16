#include "Arch/i386/Keyboard/Keyboard.h"

#include "Library/kStandardLib.h"

#include <stdbool.h>

namespace Kernel
{
	namespace
	{
		constexpr uint16_t KEYBOARD_DATA_PORT = 0x60;
		constexpr uint16_t KEYBOARD_STATUS_PORT = 0x64;
		constexpr uint16_t KEYBOARD_COMMAND_PORT = 0x64;

		constexpr uint8_t STATUS_OUTPUT_FULL = 0x01;
		constexpr uint8_t STATUS_INPUT_FULL = 0x02;

		constexpr uint32_t WAIT_TIMEOUT = 100000;

		bool WaitForInputBuffer()
		{
			for (uint32_t i = 0; i < WAIT_TIMEOUT; i++)
			{
				if (!(inb(KEYBOARD_STATUS_PORT) & STATUS_INPUT_FULL))
					return true;
			}

			return false;
		}

		bool WaitForOutputBuffer()
		{
			for (uint32_t i = 0; i < WAIT_TIMEOUT; i++)
			{
				if (inb(KEYBOARD_STATUS_PORT) & STATUS_OUTPUT_FULL)
					return true;
			}

			return false;
		}
	}

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

	Keyboard::Keyboard()
	{

	}

	bool Keyboard::InitKeyboard()
	{
		// Disable the first PS/2 port.
		if (!WaitForInputBuffer())
			return false;

		outb_p(0xAD, KEYBOARD_COMMAND_PORT);

		// Read the controller configuration byte.
		if (!WaitForInputBuffer())
			return false;

		outb_p(0x20, KEYBOARD_COMMAND_PORT);

		if (!WaitForOutputBuffer())
			return false;

		uint8_t config = inb(KEYBOARD_DATA_PORT);

		// Disable IRQ1 and disable the first port's clock.
		config &= ~0x01;
		config |= 0x10;

		if (!WaitForInputBuffer())
			return false;

		outb_p(0x60, KEYBOARD_COMMAND_PORT);

		if (!WaitForInputBuffer())
			return false;

		outb_p(config, KEYBOARD_DATA_PORT);

		// Re-enable the first PS/2 port.
		if (!WaitForInputBuffer())
			return false;

		outb_p(0xAE, KEYBOARD_COMMAND_PORT);

		// Enable keyboard scanning.
		if (!WaitForInputBuffer())
			return false;

		outb_p(0xF4, KEYBOARD_DATA_PORT);

		// Read the keyboard's acknowledgement.
		if (!WaitForOutputBuffer())
			return false;

		uint8_t response = inb(KEYBOARD_DATA_PORT);

		if (response != 0xFA)
			return false;

		// Now enable IRQ1 in the controller.
		config |= 0x01;
		config &= ~0x10;

		if (!WaitForInputBuffer())
			return false;

		outb_p(0x60, KEYBOARD_COMMAND_PORT);

		if (!WaitForInputBuffer())
			return false;

		outb_p(config, KEYBOARD_DATA_PORT);

		return true;
	}

	void Keyboard::HandleScancode(uint16_t scancode)
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

	Keyboard g_Keyboard;

	void HandleKeyboardInterrupt()
	{
		// Make sure the output buffer actually contains data.
		if (!(inb(KEYBOARD_STATUS_PORT) & STATUS_OUTPUT_FULL))
			return;

		uint8_t scancode = inb_p(KEYBOARD_DATA_PORT);

		if (scancode == 0xFA || scancode == 0xFE)
			return;

		uint8_t extended = 0x00;

		if (scancode == 0xE0)
		{
			extended = 0xE0;
			scancode = inb_p(KEYBOARD_DATA_PORT);
		}

		uint16_t keyCode = (extended << 8) | scancode;
		g_Keyboard.HandleScancode(keyCode);
	}
}
