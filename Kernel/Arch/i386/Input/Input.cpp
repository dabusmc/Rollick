#include "Input/Input.h"

#include "Arch/i386/Input/PS2Keyboard.h"

#include "Kernel.h"

namespace Kernel
{
	namespace Input
	{
		void Initialize()
		{
			static PS2Keyboard keyboard;
			g_GlobalObjects.Input_Keyboard = &keyboard;
			keyboard.Initialize();

			if (!keyboard.Ready())
			{
				// FIXME: Handle initialisation failure.
			}
		}
	}
}
