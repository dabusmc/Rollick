#pragma once

#include <stdint.h>
#include <stddef.h>

extern "C"
{

size_t strlen(const char* str);

int32_t kprintf(const char* format, ...);

}