#include "kStandardLib.h"

#include "Terminal.h"

#include <stdarg.h>

Kernel::Terminal g_Terminal;

void terminalWrite(const char* str, size_t len)
{
	g_Terminal.Write(str, len);
}

extern "C"
{

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
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

}