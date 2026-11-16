#include "Arch/i386/Interrupts/InterruptFrame.h"

#include "Arch/i386/Interrupts/PIC.h"
#include "Arch/i386/Interrupts/PIT.h"

#include "Panic.h"

#include <stdint.h>

extern "C" void interruptHandler(Kernel::Interrupts::InterruptFrame* frame)
{
	// Hardware IRQs occupy vectors 32–47.
	if (frame->InterruptNumber >= 32 && frame->InterruptNumber <= 47)
	{
		switch (frame->InterruptNumber)
		{
		case 32:
			Kernel::Interrupts::HandlePITInterrupt();
			break;
		default:
			break;
		}

		Kernel::Interrupts::SendEOI(static_cast<uint8_t>(frame->InterruptNumber));
		return;
	}

	// Anything else is treated as a CPU exception.
	Kernel::Panic("CPU Exception", frame);
}