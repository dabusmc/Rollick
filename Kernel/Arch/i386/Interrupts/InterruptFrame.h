#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Interrupts
	{
		struct InterruptFrame
		{
			// General-purpose registers
			uint32_t EDI;
			uint32_t ESI;
			uint32_t EBP;
			uint32_t ESP;
			uint32_t EBX;
			uint32_t EDX;
			uint32_t ECX;
			uint32_t EAX;

			// Segment registers
			uint32_t GS;
			uint32_t FS;
			uint32_t ES;
			uint32_t DS;

			// Interrupt information
			uint32_t InterruptNumber;
			uint32_t ErrorCode;

			// CPU-pushed state
			uint32_t EIP;
			uint32_t CS;
			uint32_t EFlags;
		};
	}
}

extern "C" void interruptHandler(Kernel::Interrupts::InterruptFrame* frame);