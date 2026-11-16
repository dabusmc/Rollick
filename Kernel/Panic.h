#pragma once

namespace Kernel
{
	[[noreturn]] void Panic(const char* message, const char* file, int line);
}

#define KERNEL_PANIC(message) Kernel::Panic(message, __FILE__, __LINE__)