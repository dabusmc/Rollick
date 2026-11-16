#pragma once

#include <stddef.h>
#include <stdint.h>

namespace Kernel
{
	class Terminal
	{
	public:
		Terminal();

		void Clear();

		void Write(const char* data);
		void Write(const char* data, size_t size);

		void SetCursorEnabled(bool enabled);

	private:
		void PutChar(char c);
		void Scroll();

		void EnableCursor();
		void DisableCursor();
		void UpdateCursorPosition();

	private:
		size_t m_Row;
		size_t m_Column;
		uint8_t m_Color;
		uint16_t* m_Buffer = nullptr;

		bool m_CursorEnabled = false;
	};
}