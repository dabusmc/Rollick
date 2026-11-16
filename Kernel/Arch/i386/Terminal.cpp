#include "Terminal.h"

#include "VGA.h"

#include "Library/kStandardLib.h"

extern "C"
{
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void _terminalPutEntryAt(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vgaEntry(c, color);
}

void _terminalScroll(void)
{
	// Clear Top Line
	for (size_t x = 0; x < VGA_WIDTH; x++)
	{
		terminal_buffer[x] = vgaEntry(' ', terminal_color);
	}

	// Move all current lines up
	for (size_t y = 1; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			const size_t newIndex = (y - 1) * VGA_WIDTH + x;

			terminal_buffer[newIndex] = terminal_buffer[index];
			terminal_buffer[index] = vgaEntry(' ', terminal_color);
		}
	}

	// Update variables
	terminal_row -= 1;
}

void terminalInitialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vgaEntryColor(VGAColor_LightGray, VGAColor_Black);

	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vgaEntry(' ', terminal_color);
		}
	}
}

void terminalPutChar(char c)
{
	if(c == '\n')
	{
		if (++terminal_row == VGA_HEIGHT)
		{
			_terminalScroll();
		}
		terminal_column = 0;
		return;
	}

	_terminalPutEntryAt(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			_terminalScroll();
		}
	}
}

void terminalWrite(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminalPutChar(data[i]);
	}
}

void terminalWriteString(const char* data)
{
	terminalWrite(data, strlen(data));
}

}