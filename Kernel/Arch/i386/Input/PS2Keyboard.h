#pragma once

#include "Input/Keyboard.h"

namespace Kernel
{
	class PS2Keyboard : public Keyboard
	{
	public:
		void Initialize() override;
	};

	void HandleKeyboardInterrupt();
}