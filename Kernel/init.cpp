#include "Arch/i386/Interrupts/IDT.h"
#include "Arch/i386/Interrupts/PIC.h"
#include "Arch/i386/Interrupts/PIT.h"
#include "Arch/i386/Keyboard/Keyboard.h"

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
	bool keyboardReady = Kernel::g_Keyboard.InitKeyboard();

	if (keyboardReady)
		kprintf("Initialized keyboard\n");
	else
		kprintf("WARNING: Keyboard initialization failed\n");

	Kernel::Interrupts::SetIRQMask(0, false);

	if (keyboardReady)
	{
		Kernel::Interrupts::SetIRQMask(1, false);
	}

	kprintf("PIC master mask: %x\n", inb(0x21));

	asm volatile("sti" ::: "memory");
	kprintf("Enabled interrupts\n");
}

extern "C" void kernelMain(void)
{
	initInterrupts();

	while (true)
	{
		asm volatile("hlt");
	}
}
