#pragma once

#include "Input/KeyCode.h"
#include "Input/KeyEvent.h"

#include <stdint.h>

namespace Kernel
{
	class Keyboard
	{
	public:
		~Keyboard() = default;
		virtual void Initialize() = 0;

		[[nodiscard]] bool Ready() const { return m_IsReady; }
		[[nodiscard]] char TranslateKey(KeyCode keycode) const;

		void InterpretScancode(uint16_t scancode);
		void ProcessKeyEvent(const KeyEvent& event);

	protected:
		bool m_IsReady = false;
		bool m_IsShifted = false;
	};
}
