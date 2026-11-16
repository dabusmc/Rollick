#pragma once

namespace Kernel::Interrupts
{
	struct InterruptFrame;
}

namespace Kernel
{
	[[noreturn]] void Panic(const char* message, const char* file, int line);
	[[noreturn]] void Panic(const char* message, const Interrupts::InterruptFrame* frame);
}

#define KERNEL_PANIC(message) Kernel::Panic(message, __FILE__, __LINE__)