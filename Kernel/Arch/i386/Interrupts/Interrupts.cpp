#include "InterruptFrame.h"

#include "Panic.h"

extern "C" void interruptHandler(Kernel::Interrupts::InterruptFrame* frame)
{
	Kernel::Panic("CPU Exception", frame);
}