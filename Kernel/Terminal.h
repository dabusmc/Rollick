#pragma once

#include <stddef.h>

extern "C"
{

void terminalInitialize();
void terminalPutChar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteString(const char* data);

}