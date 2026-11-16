#include "Input/KeyEventQueue.h"

#include <stdint.h>

namespace Kernel
{
	bool KeyEventQueue::Push(const KeyEvent& event)
	{
		size_t next = (m_Head + 1) % CAPACITY;

		if (next == m_Tail)
			return false;

		m_Events[m_Head] = event;

		// Ensure the event is written before publishing the new head.
		asm volatile("" ::: "memory");

		m_Head = next;
		return true;
	}

	bool KeyEventQueue::Pop(KeyEvent& event)
	{
		uint32_t flags;

		// Save the current interrupt state, then disable interrupts.
		asm volatile(
			"pushfl\n\t"
			"popl %0\n\t"
			"cli"
			: "=r"(flags)
			:
			: "memory"
		);

		if (m_Head == m_Tail)
		{
			if (flags & (1u << 9))
				asm volatile("sti" ::: "memory");

			return false;
		}

		// Ensure we read the event after observing the updated head.
		asm volatile("" ::: "memory");

		event = m_Events[m_Tail];
		m_Tail = (m_Tail + 1) % CAPACITY;

		// Restore interrupts only if they were enabled before Pop().
		if (flags & (1u << 9))
			asm volatile("sti" ::: "memory");

		return true;
	}

	bool KeyEventQueue::Empty() const
	{
		return m_Head == m_Tail;
	}

	bool KeyEventQueue::Full() const
	{
		return (m_Head + 1) % CAPACITY == m_Tail;
	}
}