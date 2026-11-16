#include "Arch/i386/Interrupts/PIT.h"

#include "Library/kStandardLib.h"

namespace Kernel
{
	namespace Interrupts
	{
		namespace
		{
			constexpr uint16_t PIT_COMMAND = 0x43;
			constexpr uint16_t PIT_CHANNEL0 = 0x40;

			constexpr uint32_t PIT_INPUT_FREQUENCY = 1193182;

			volatile uint32_t s_Ticks = 0;
		}

		void InitializePIT(uint32_t frequency)
		{
			if (frequency == 0)
			{
				return;
			}

			uint32_t divisor = PIT_INPUT_FREQUENCY / frequency;

			if (divisor == 0 || divisor > 65535)
			{
				return;
			}

			// Channel 0, low/high byte access, mode 3, binary mode.
			outb(0x36, PIT_COMMAND);

			// Send the divisor, low byte first.
			outb(static_cast<uint8_t>(divisor & 0xFF), PIT_CHANNEL0);

			// Then the high byte.
			outb(static_cast<uint8_t>((divisor >> 8) & 0xFF), PIT_CHANNEL0);
		}

		void HandlePITInterrupt()
		{
			s_Ticks++;
		}

		uint32_t GetPITTicks()
		{
			return s_Ticks;
		}
	}
}