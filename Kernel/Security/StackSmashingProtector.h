#pragma once

#include <stdint.h>

extern "C"
{
extern uintptr_t __stack_chk_guard;

[[noreturn]] void __stack_chk_fail();
}

namespace Kernel
{
	__attribute__((no_stack_protector)) void InitializeStackSmashingProtector();
}