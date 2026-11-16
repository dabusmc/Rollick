#pragma once

#include "Input/KeyEvent.h"

#include <stdint.h>

namespace Kernel
{
	class PS2Scancode
	{
	public:
		bool Decode(uint8_t scancode, KeyEvent& event);

	private:
		bool m_Extended = false;
	};
}