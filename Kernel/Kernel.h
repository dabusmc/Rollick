#pragma once
#include "Terminal.h"
#include "Arch/i386/Serial/Serial.h"
#include "Input/Keyboard.h"

namespace Kernel
{
	struct GlobalObjects
	{
		Keyboard* Input_Keyboard;
	};

	extern GlobalObjects g_GlobalObjects;
}
