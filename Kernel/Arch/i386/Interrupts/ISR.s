.section .text

.global default_interrupt_handler
.type default_interrupt_handler, @function

default_interrupt_handler:
    /* Disable maskable interrupts. */
    cli

1:
    /* Halt the processor. */
    hlt

    /* If execution resumes, halt again. */
    jmp 1b

.size default_interrupt_handler, . - default_interrupt_handler