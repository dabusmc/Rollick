#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Interrupts
	{
		void InitializePIT(uint32_t frequency);
		void HandlePITInterrupt();
		uint32_t GetPITTicks();
	}
}