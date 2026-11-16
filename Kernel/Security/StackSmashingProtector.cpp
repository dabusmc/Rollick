#include "StackSmashingProtector.h"

#include "Panic.h"

extern "C"
{

uintptr_t __stack_chk_guard = 0;

[[noreturn]] void __stack_chk_fail(void)
{
	KERNEL_PANIC("Stack smashing detected!");
}

}

namespace Kernel
{
	__attribute__((no_stack_protector)) void InitializeStackSmashingProtector()
	{
		// FIXME: This should be a randomized values every time it is initialized
		__stack_chk_guard = 0xe2dee396;
	}
}