#pragma once

#include <stddef.h>

namespace Kernel
{
	class Serial
	{
	public:
		Serial();

		void Write(const char* data, size_t size);

	private:
		void PutChar(char c);
	};
}