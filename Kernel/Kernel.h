#pragma once

#include "Input/Keyboard.h"
#include "Input/KeyEventQueue.h"

namespace Kernel
{
	struct GlobalObjects
	{
		Keyboard* Input_Keyboard = nullptr;
		KeyEventQueue Input_KeyboardEvents;
	};

	extern GlobalObjects g_GlobalObjects;
}
