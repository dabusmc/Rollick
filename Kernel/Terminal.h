#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Kernel
{
	class Terminal
	{
	public:
		Terminal();

		void Write(const char* data);
		void Write(const char* data, size_t size);

	private:
		void PutChar(char c);
		void Scroll();

	private:
		size_t m_Row;
		size_t m_Column;
		uint8_t m_Color;
		uint16_t* m_Buffer = nullptr;
	};
}