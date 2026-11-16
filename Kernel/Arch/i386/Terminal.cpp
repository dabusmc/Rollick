#include "Terminal.h"

#include "VGA.h"

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

		for (size_t y = 0; y < VGA_HEIGHT; y++)
		{
			for (size_t x = 0; x < VGA_WIDTH; x++)
			{
				const size_t index = y * VGA_WIDTH + x;
				m_Buffer[index] = vgaEntry(' ', m_Color);
			}
		}
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

	void Terminal::PutChar(char c)
	{
		if(c == '\n')
		{
			if (++m_Row == VGA_HEIGHT)
			{
				Scroll();
			}
			m_Column = 0;
			return;
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
	}
}
