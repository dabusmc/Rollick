#pragma once

#include "Input/KeyCode.h"

namespace Kernel
{
	enum class KeyState
	{
		Pressed,
		Released
	};

	struct KeyEvent
	{
		KeyCode Key;
		KeyState State;
	};
}