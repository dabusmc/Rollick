#pragma once

#include "Input/KeyCode.h"
#include "Input/KeyEvent.h"

namespace Kernel
{
	class Keyboard
	{
	public:
		~Keyboard() = default;
		virtual void Initialize() = 0;

		[[nodiscard]] bool Ready() const { return m_IsReady; }
		[[nodiscard]] char TranslateKey(KeyCode keycode) const;

		void ProcessKeyEvent(const KeyEvent& event);

	protected:
		bool m_IsReady = false;

		bool m_LeftShiftPressed = false;
		bool m_RightShiftPressed = false;
		bool m_CapsLockEnabled = false;
		bool m_CapsLockPressed = false;
	};
}
