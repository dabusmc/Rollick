#pragma once

#include "Input/Keyboard.h"
#include "Input/KeyEventQueue.h"

#include "Memory/Heap.h"
#include "Memory/PhysicalMemoryManager.h"

namespace Kernel
{
	struct GlobalObjects
	{
		Keyboard* Input_Keyboard = nullptr;
		KeyEventQueue Input_KeyboardEvents;

		Memory::PhysicalMemoryManager Memory_PMM;
		Memory::Heap Memory_Heap;
	};

	extern GlobalObjects g_GlobalObjects;
}
