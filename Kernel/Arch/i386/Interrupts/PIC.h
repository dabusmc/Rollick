#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Interrupts
	{
		void InitializePIC();
		void SendEOI(uint8_t interruptNumber);
		void SetIRQMask(uint8_t irq, bool masked);
	}
}