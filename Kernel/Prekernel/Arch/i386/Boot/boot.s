
/*
 * Multiboot header constants.
 *
 * These values tell a Multiboot-compatible bootloader how to load and start the kernel.
 */
.set ALIGN,    1<<0          /* Request that loaded modules are page-aligned. */
.set MEMINFO,  1<<1          /* Request memory map information from the bootloader. */
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002    /* Multiboot 1 magic number. */
.set CHECKSUM, -(MAGIC + FLAGS) /* Makes MAGIC + FLAGS + CHECKSUM equal zero. */

/*
 * Our kernel's higher-half virtual address offset.
 *
 * Physical address + KERNEL_VMA = higher-half virtual address.
 * For example: 0x00200000 + 0xC0000000 = 0xC0200000.
 */
.set KERNEL_VMA, 0xC0000000


/*
 * Multiboot header.
 *
 * The bootloader searches for this header near the beginning of
 * the kernel file to identify it as a Multiboot kernel.
 */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/*
 * Bootstrap memory allocations.
 *
 * These are placed in .boot.bss so they reside at low physical
 * addresses and can be accessed before paging is enabled.
 */
.section .boot.bss,"aw",@nobits

/* Reserve a 16 KiB stack. The x86 stack grows downwards. */
.align 16
stack_bottom:
.skip 16384
stack_top:                  /* Initial ESP points here. */

/*
 * Page directory: 1024 entries, each 4 bytes (4 KiB total).
 * Must be aligned to a 4 KiB boundary.
 */
.align 4096
page_directory:
.skip 4096

/*
 * First page table: 1024 entries, each 4 bytes (4 KiB total).
 * Each entry maps one 4 KiB physical page.
 */
.align 4096
first_page_table:
.skip 4096


/*
 * Bootstrap entry point.
 *
 * The linker places this section at a low physical address.
 * The bootloader jumps here with paging disabled.
 */
.section .boot.text,"ax"
.global _start
.type _start, @function
_start:

	/* Set ESP to the top of the allocated bootstrap stack. */
	mov $stack_top, %esp

	/*
	 * Clear the page directory and page table.
	 *
	 * EDI = destination address.
	 * ECX = number of 32-bit values to write.
	 * EAX = value to write (zero).
	 *
	 * 2048 values * 4 bytes = 8192 bytes (8 KiB).
	 */
	mov $page_directory, %edi
	mov $2048, %ecx
	xor %eax, %eax

	/* Ensure string instructions increment their destination pointers. */
	cld

	/* Repeat STOSL ECX times, writing EAX to [EDI]. */
	rep stosl

	/*
	 * Identity-map the first 4 MiB using 4 KiB pages.
	 *
	 * EDI = current page-table entry.
	 * EAX = physical address plus page flags.
	 * ECX = number of page-table entries to create.
	 */
	mov $first_page_table, %edi
	mov $0x00000003, %eax  /* Bit 0: Present, Bit 1: Writable. */
	mov $1024, %ecx

1:
	/* Store the current page-table entry and advance EDI by 4 bytes. */
	stosl

	/* Advance the physical address by one 4 KiB page. */
	add $0x1000, %eax

	/* Decrement ECX and repeat until all 1024 entries are created. */
	loop 1b

	/*
	 * Connect the page table to the page directory.
	 *
	 * PDE 0 maps virtual addresses 0x00000000–0x003FFFFF.
	 * PDE 768 maps virtual addresses 0xC0000000–0xC03FFFFF.
	 *
	 * Both entries point to the same page table, giving us
	 * an identity mapping and a higher-half mapping.
	 */
	mov $first_page_table, %eax

	/* Mark the page-table pointer as Present and Writable. */
	or $0x3, %eax

	/* Set page-directory entry 0. */
	mov %eax, page_directory

	/* Set page-directory entry 768 (768 * 4 bytes per entry). */
	mov %eax, page_directory + (768 * 4)

	/*
	 * Load CR3 with the physical address of the page directory.
	 *
	 * CR3 tells the processor where to find the current page directory.
	 * This must be a physical address, not a higher-half virtual address.
	 */
	mov $page_directory, %eax
	mov %eax, %cr3

	/*
	 * Enable paging by setting bit 31 (PG) in CR0.
	 *
	 * From this point onwards, memory accesses are translated
	 * through the page directory and page tables.
	 */
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	/*
	 * Jump to the higher-half entry point.
	 *
	 * We use an indirect jump because higher_half_entry has a
	 * higher-half virtual address, whereas this bootstrap code
	 * is executing at a low address.
	 *
	 * The identity mapping keeps the current instruction address
	 * valid until this jump takes us into the higher half.
	 */
	mov $higher_half_entry, %eax
	jmp *%eax

.size _start, . - _start


/*
 * Higher-half kernel entry point.
 *
 * Unlike _start, this function is linked at a higher-half virtual
 * address. It can now safely call the normal C++ kernel functions.
 */
.section .text
.global higher_half_entry
.type higher_half_entry, @function

higher_half_entry:

    /*
     * Switch to the higher-half alias of the existing stack.
     *
     * The physical stack hasn't moved. We're just accessing it
     * through its higher-half virtual address instead.
     */
    mov $(stack_top + KERNEL_VMA), %esp

    /*
     * Run the early kernel initialisation.
     *
     * prekernelInit() initialises the stack-smashing protector
     * and executes global C++ constructors.
     */
    call prekernelInit

    /* Enter the main kernel function. */
    call kernelMain

    /*
     * kernelMain should never return.
     * If it does, disable interrupts and halt the processor.
     */
    cli

1:
    /* Halt the CPU until an interrupt or other wake-up event occurs. */
    hlt

    /* Return to the halt instruction if execution resumes. */
    jmp 1b

.size higher_half_entry, . - higher_half_entry
