#include "Library/kStandardLib.h"

#include "Arch/i386/Serial.h"

#include "Memory/Heap.h"

#include "Kernel.h"
#include "Terminal.h"

#include <stdarg.h>

Kernel::Terminal g_Terminal;
Kernel::Serial g_Serial;

namespace
{
	void terminalWrite(const char* str, size_t len)
	{
		g_Terminal.Write(str, len);
		g_Serial.Write(str, len);
	}

	void* heapAllocate(size_t size)
	{
		return Kernel::g_GlobalObjects.Memory_Heap.Allocate(size);
	}

	void heapFree(void* ptr)
	{
		Kernel::g_GlobalObjects.Memory_Heap.Free(ptr);
	}
}

extern "C"
{

void clrscr()
{
	g_Terminal.Clear();
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

size_t itoa(int32_t value, char* buffer)
{
	size_t index = 0;
	bool negative = value < 0;

	// Handle zero separately
	if (value == 0)
	{
		buffer[index++] = '0';
	}
	else
	{
		uint32_t magnitude = negative ? 0u - static_cast<uint32_t>(value) : static_cast<uint32_t>(value);
		while (magnitude > 0)
		{
			buffer[index++] = '0' + (magnitude % 10);
			magnitude /= 10;
		}

		if (negative)
		{
			buffer[index++] = '-';
		}

		for (size_t i = 0; i < index / 2; i++)
		{
			char temp = buffer[i];
			buffer[i] = buffer[index - i - 1];
			buffer[index - i - 1] = temp;
		}
	}

	buffer[index] = '\0';

	return index;
}

size_t utoa(uint32_t value, char* buffer)
{
	size_t index = 0;

	// Handle zero separately
	if (value == 0)
	{
		buffer[index++] = '0';
	}
	else
	{
		uint32_t magnitude = value;
		while (magnitude > 0)
		{
			buffer[index++] = '0' + (magnitude % 10);
			magnitude /= 10;
		}

		for (size_t i = 0; i < index / 2; i++)
		{
			char temp = buffer[i];
			buffer[i] = buffer[index - i - 1];
			buffer[index - i - 1] = temp;
		}
	}

	buffer[index] = '\0';

	return index;
}

int32_t putchar(int ic)
{
	char c = (char) ic;
	terminalWrite(&c, sizeof(c));

	return (unsigned char)c;
}

int32_t kprintf(const char* format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int32_t written = 0;

	while (*format != '\0')
	{
		if (*format != '%')
		{
			putchar(*format++);
			written++;
			continue;
		}

		format++;

		switch (*format)
		{
		case '%':
			{
				putchar('%');
				written++;
				break;
			}
		case 'c':
			{
				char c = (char) va_arg(parameters, int);
				putchar(c);
				written++;
				break;
			}
		case 's':
			{
				const char* str = va_arg(parameters, const char*);

				if (str == nullptr)
				{
					str = "(null)";
				}

				size_t len = strlen(str);
				terminalWrite(str, len);
				written += len;
				break;
			}
		case 'd':
		case 'i':
			{
				int32_t value = va_arg(parameters, int);

				char buffer[12]; // NOTE: 12 is enough to store '-2147483648' including the null terminator
				size_t len = itoa(value, buffer);
				terminalWrite(buffer, len);
				written += len;
				break;
			}
		case 'u':
			{
				uint32_t value = va_arg(parameters, unsigned int);

				char buffer[11]; // NOTE: 11 is enough to store '4294967295' including the null terminator
				size_t len = utoa(value, buffer);
				terminalWrite(buffer, len);
				written += len;
				break;
			}
		case 'X':
			{
				int64_t value = va_arg(parameters, int64_t);
				const char* digits = "0123456789ABCDEF";
				for (int i = 28; i >= 0; i -= 4)
				{
					putchar(digits[(value >> i) & 0xF]);
					written++;
				}
				break;
			}
		case 'x':
			{
				int64_t value = va_arg(parameters, int64_t);
				const char* digits = "0123456789abcdef";
				for (int i = 28; i >= 0; i -= 4)
				{
					putchar(digits[(value >> i) & 0xF]);
					written++;
				}
				break;
			}
		default:
			{
				putchar('%');
				putchar(*format);
				written += 2;
				break;
			}
		}

		if (*format != '\0')
		{
			format++;
		}
	}

	va_end(parameters);

	return written;
}

void* kmalloc(size_t size)
{
	return heapAllocate(size);
}

void kfree(void* p)
{
	heapFree(p);
}

void io_wait()
{
	asm volatile ("outb %al, $0x80");
}

uint8_t inb(uint16_t port)
{
	uint8_t result;
	asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
	return result;
}

uint8_t inb_p(uint16_t port)
{
	uint8_t result = inb(port);
	io_wait();
	return result;
}

void outb(uint8_t value, uint16_t port)
{
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outb_p(uint8_t value, uint16_t port)
{
	outb(value, port);
	io_wait();
}

}