#include "Kernel.h"
#include "Arch/i386/Interrupts/IDT.h"
#include "Arch/i386/Interrupts/PIC.h"
#include "Arch/i386/Interrupts/PIT.h"
#include "Arch/i386/Input/PS2Keyboard.h"
#include "Input/Input.h"

#include "Library/kStandardLib.h"

#define PIT_FREQUENCY 100

void initInterrupts()
{
	Kernel::Interrupts::InitializeIDT();
	kprintf("Initialized IDT\n");
	Kernel::Interrupts::InitializePIC();
	kprintf("Initialized PIC\n");

	Kernel::Interrupts::InitializePIT(PIT_FREQUENCY);
	kprintf("Initialized PIT at a frequency of %d\n", PIT_FREQUENCY);

	Kernel::Interrupts::SetIRQMask(0, false);
	if (Kernel::g_GlobalObjects.Input_Keyboard->Ready())
	{
		Kernel::Interrupts::SetIRQMask(1, false);
	}

	asm volatile("sti" ::: "memory");
	kprintf("Enabled interrupts\n");
}

extern "C" void kernelMain(void)
{
	Kernel::Input::Initialize();
	if (Kernel::g_GlobalObjects.Input_Keyboard->Ready())
	{
		kprintf("Initialized keyboard\n");
	}
	else
	{
		kprintf("WARNING: Keyboard initialization failed\n");
	}

	initInterrupts();

	while (true)
	{
		asm volatile("hlt");
	}
}
