#include "kStandardLib.h"

#include <stdarg.h>

extern "C"
{

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int32_t kprintf(const char* format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	while (*format != '\0')
	{

	}

	va_end(parameters);
	return 0;
}

}