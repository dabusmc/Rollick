#pragma once

#include "Input/KeyEvent.h"

#include <stddef.h>

namespace Kernel
{
	// FIXME: Abstract out the concept of a Queue and put it in RoLib
	class KeyEventQueue
	{
	public:
		static constexpr size_t CAPACITY = 128;

		bool Push(const KeyEvent& event);
		bool Pop(KeyEvent& event);

		[[nodiscard]] bool Empty() const;
		[[nodiscard]] bool Full() const;

	private:
		KeyEvent m_Events[CAPACITY] = {};

		volatile size_t m_Head = 0;
		volatile size_t m_Tail = 0;
	};
}