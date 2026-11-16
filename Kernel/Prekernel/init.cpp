#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

using InitFunction = void (*)();

extern "C"
{

extern InitFunction __init_array_start[];
extern InitFunction __init_array_end[];

void prekernelInit()
{
    for (InitFunction* function = __init_array_start; function != __init_array_end; ++function)
    {
        (*function)();
    }
}

}