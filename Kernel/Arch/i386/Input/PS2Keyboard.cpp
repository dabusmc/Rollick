#include "Arch/i386/Input/PS2Keyboard.h"

#include "Library/kStandardLib.h"

#include "Kernel.h"

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

	void PS2Keyboard::Initialize()
	{
		// Disable the first PS/2 port.
		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0xAD, KEYBOARD_COMMAND_PORT);

		// Read the controller configuration byte.
		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0x20, KEYBOARD_COMMAND_PORT);

		if (!WaitForOutputBuffer())
		{
			m_IsReady = false;
			return;
		}

		uint8_t config = inb(KEYBOARD_DATA_PORT);

		// Disable IRQ1 and disable the first port's clock.
		config &= ~0x01;
		config |= 0x10;

		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0x60, KEYBOARD_COMMAND_PORT);

		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(config, KEYBOARD_DATA_PORT);

		// Re-enable the first PS/2 port.
		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0xAE, KEYBOARD_COMMAND_PORT);

		// Enable keyboard scanning.
		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0xF4, KEYBOARD_DATA_PORT);

		// Read the keyboard's acknowledgement.
		if (!WaitForOutputBuffer())
		{
			m_IsReady = false;
			return;
		}

		uint8_t response = inb(KEYBOARD_DATA_PORT);

		if (response != 0xFA)
		{
			m_IsReady = false;
			return;
		}

		// Now enable IRQ1 in the controller.
		config |= 0x01;
		config &= ~0x10;

		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(0x60, KEYBOARD_COMMAND_PORT);

		if (!WaitForInputBuffer())
		{
			m_IsReady = false;
			return;
		}

		outb_p(config, KEYBOARD_DATA_PORT);

		m_IsReady = true;
	}

	void HandleKeyboardInterrupt()
	{
		// Make sure the output buffer actually contains data.
		if (!(inb(KEYBOARD_STATUS_PORT) & STATUS_OUTPUT_FULL))
		{
			return;
		}

		uint8_t scancode = inb_p(KEYBOARD_DATA_PORT);

		if (scancode == 0xFA || scancode == 0xFE)
		{
			return;
		}

		uint8_t extended = 0x00;
		if (scancode == 0xE0)
		{
			extended = 0xE0;
			scancode = inb_p(KEYBOARD_DATA_PORT);
		}

		uint16_t keyCode = (extended << 8) | scancode;
		g_GlobalObjects.Input_Keyboard->InterpretScancode(keyCode);
	}
}
