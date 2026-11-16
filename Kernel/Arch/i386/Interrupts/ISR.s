.section .text

.global isr_common
.type isr_common, @function

isr_common:
    cld

    # Save segment registers
    push %ds
    push %es
    push %fs
    push %gs

    # Save general-purpose registers
    pusha

    # Switch to kernel data segments
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Preserve the address of our saved CPU state
    mov %esp, %ebx

    # Align the stack to 16 bytes for the C++ call
    andl $-16, %esp
    subl $12, %esp
    pushl %ebx

    # Call the C++ interrupt handler
    call interruptHandler

    # Restore the original interrupt stack
    movl %ebx, %esp

    # Restore general-purpose registers
    popa

    # Restore segment registers
    pop %gs
    pop %fs
    pop %es
    pop %ds

    # Discard exception number and error code
    addl $8, %esp

    # Return from interrupt
    iret

.macro ISR_NOERR n
.global isr\n
isr\n:
    pushl $0          # Dummy error code
    pushl $\n         # Exception number
    jmp isr_common
.endm

.macro ISR_ERR n
.global isr\n
isr\n:
    pushl $\n         # Exception number
    jmp isr_common
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

ISR_ERR 8

ISR_NOERR 9

ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14

ISR_NOERR 15
ISR_NOERR 16

ISR_ERR 17

ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR 21

ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28

ISR_ERR 29
ISR_ERR 30

ISR_NOERR 31

.macro IRQ n
.global irq\n
irq\n:
    pushl $0          # Dummy error code
    pushl $(32 + \n)  # Interrupt vector
    jmp isr_common
.endm

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
