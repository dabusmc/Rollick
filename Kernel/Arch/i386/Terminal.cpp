#include "Terminal.h"

#include "Arch/i386/VGA.h"

#include "Library/kStandardLib.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

namespace Kernel
{
	Terminal::Terminal()
		: m_Row(0), m_Column(0), m_Color(0)
	{
		m_Color = vgaEntryColor(VGAColor_LightGray, VGAColor_Black);
		m_Buffer = reinterpret_cast<uint16_t*>(VGA_MEMORY);
		Clear();
		EnableCursor();
	}

	void Terminal::Clear()
	{
		for (size_t y = 0; y < VGA_HEIGHT; y++)
		{
			for (size_t x = 0; x < VGA_WIDTH; x++)
			{
				const size_t index = y * VGA_WIDTH + x;
				m_Buffer[index] = vgaEntry(' ', m_Color);
			}
		}

		m_Row = 0;
		m_Column = 0;
	}

	void Terminal::Write(const char* data)
	{
		Write(data, strlen(data));
	}

	void Terminal::Write(const char* data, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			PutChar(data[i]);
		}
	}

	void Terminal::SetCursorEnabled(bool enabled)
	{
		if (enabled)
		{
			EnableCursor();
		}
		else
		{
			DisableCursor();
		}
	}

	void Terminal::PutChar(char c)
	{
		switch (c)
		{
		case '\n':
			{
				if (++m_Row == VGA_HEIGHT)
				{
					Scroll();
				}
				m_Column = 0;
				UpdateCursorPosition();
				return;
			}
		case '\b':
			{
				// FIXME: Terminal can only backspace on the current line
				if (m_Column == 0)
				{
					return;
				}

				m_Column--;

				const size_t index = m_Row * VGA_WIDTH + m_Column;
				m_Buffer[index] = vgaEntry(' ', m_Color);
				UpdateCursorPosition();
				return;
			}
		default:
			break;
		}

		const size_t index = m_Row * VGA_WIDTH + m_Column;
		m_Buffer[index] = vgaEntry(c, m_Color);

		if (++m_Column == VGA_WIDTH)
		{
			m_Column = 0;
			if (++m_Row == VGA_HEIGHT)
			{
				Scroll();
			}
		}
		UpdateCursorPosition();
	}

	void Terminal::Scroll()
	{
		// Clear Top Line
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			m_Buffer[x] = vgaEntry(' ', m_Color);
		}

		// Move all current lines up
		for (size_t y = 1; y < VGA_HEIGHT; y++)
		{
			for (size_t x = 0; x < VGA_WIDTH; x++)
			{
				const size_t index = y * VGA_WIDTH + x;
				const size_t newIndex = (y - 1) * VGA_WIDTH + x;

				m_Buffer[newIndex] = m_Buffer[index];
				m_Buffer[index] = vgaEntry(' ', m_Color);
			}
		}

		// Update variables
		m_Row -= 1;
		UpdateCursorPosition();
	}

	void Terminal::EnableCursor()
	{
		outb(0x0A, 0x3D4);
		outb((inb(0x3D5) & 0xC0) | 1, 0x3D5);

		outb(0x0B, 0x3D4);
		outb((inb(0x3D5) & 0xE0) | 1, 0x3D5);
	}

	void Terminal::DisableCursor()
	{
		outb(0x0A, 0x3D4);
		outb(0x20, 0x3D5);
	}

	void Terminal::UpdateCursorPosition()
	{
		uint16_t pos = (m_Row + 1) * VGA_WIDTH + m_Column;

		outb(0x0F, 0x3D4);
		outb(pos & 0xFF, 0x3D5);
		outb(0x0E, 0x3D4);
		outb((pos >> 8) & 0xFF, 0x3D5);
	}
}
