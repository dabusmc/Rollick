#pragma once

#include <stdint.h>
#include <stddef.h>

extern "C"
{

void clrscr();

size_t strlen(const char* str);

size_t itoa(int32_t value, char* buffer);
int32_t putchar(int ic);
int32_t kprintf(const char* format, ...);

}