#include "Security/StackSmashingProtector.h"

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

using InitFunction = void (*)();

extern "C"
{

extern InitFunction __init_array_start[];
extern InitFunction __init_array_end[];

__attribute__((no_stack_protector)) void prekernelInit()
{
    Kernel::InitializeStackSmashingProtector();

    for (InitFunction* function = __init_array_start; function != __init_array_end; ++function)
    {
        (*function)();
    }
}

}