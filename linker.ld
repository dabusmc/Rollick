OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)

KERNEL_VMA = 0xC0000000;
KERNEL_LMA = 0x00200000;

SECTIONS
{
    /* Physical bootstrap sections */
    . = KERNEL_LMA;

    .boot ALIGN(4K) :
    {
        KEEP(*(.multiboot))

        *(.boot.text)
        *(.boot.text.*)

        *(.boot.rodata*)
        *(.boot.data*)
    }

    /* Bootstrap stack and other uninitialised data */
    .boot.bss ALIGN(16) (NOLOAD) :
    {
        *(.boot.bss)
        *(.boot.bss.*)
    }

    /*
     * Move the virtual location counter into
     * the higher-half address space.
     */
    . = ALIGN(., 4K) + KERNEL_VMA;

    /* Kernel code */
    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
    {
        *(.text)
        *(.text.*)
    }

    /* Read-only data */
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* Global constructors */
    .init_array ALIGN(4) : AT(ADDR(.init_array) - KERNEL_VMA)
    {
        PROVIDE_HIDDEN(__init_array_start = .);

        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))

        PROVIDE_HIDDEN(__init_array_end = .);
    }

    /* Initialised data */
    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
    {
        *(.data)
        *(.data.*)
    }

    /* Uninitialised data */
    .bss ALIGN(4K) (NOLOAD) : AT(ADDR(.bss) - KERNEL_VMA)
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    __kernel_virtual_start = ADDR(.text);
    __kernel_virtual_end = .;

    __kernel_physical_start = KERNEL_LMA;
    __kernel_physical_end = __kernel_virtual_end - KERNEL_VMA;
}
