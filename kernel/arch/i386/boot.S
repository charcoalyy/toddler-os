.set ALIGN,     1 << 0              /* ensures kernel is loaded at proper memory handle */
.set MEMINFO,   1<<1                /* gives kernel memory map information */
.set FLAGS,     ALIGN | MEMINFO     /* passes both the above along to kernel */
.set MAGIC,     0x1BADB002          /* code that bootloader looks for to identify kernel */
.set CHECKSUM,  -(MAGIC + FLAGS)    /* double checks the above for corruption */

/* declare multiboot header  */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* defines memory for a stack */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* executable code where bootloader hands control to kernel */
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp            /* set up stack pointer */
    call kernel_main                /* call kernel */
    cli                             /* disable interrupts */
1:  hlt                             /* halt (sleep) the CPU (since now kernel is in infinite loop) */
    jmp 1b                          /* keep repeating the line above */
.size _start, . - _start
