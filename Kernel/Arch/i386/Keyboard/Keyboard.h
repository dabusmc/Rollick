#pragma once

#include <stdint.h>
#include <stdbool.h>

namespace Kernel
{
	class Keyboard
	{
	public:
		Keyboard();

		bool InitKeyboard();
		void HandleScancode(uint16_t scancode);
	};

	extern Keyboard g_Keyboard;

	void HandleKeyboardInterrupt();
}