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

void io_wait();

uint8_t inb(uint16_t port);
uint8_t inb_p(uint16_t port);

void outb(uint8_t value, uint16_t port);
void outb_p(uint8_t value, uint16_t port);

}