@ Created by Zhigang An on Mar. 6, 2017.
@ Case Western Reserve University
@
@ Sample Code for assembling.
@ Assemble: gcc -Wall -Wextra -Werror -c HelloWorld.s -o HelloWorld_asm.o
@ Link: ld HelloWorld_asm.o -o HelloWorld_asm
@ Run: ./HelloWorld_asm

.text
.global _start
_start:                         @ Entry point.
    mov r0, #1                  @ Display text in stdout.
    ldr r1, =message            @ Load memory address of the string.
    ldr r2, =len                @ Load length of string.
    mov r7, #4                  @ Pass system call SYS_WRITE to register r7.
    swi 0

@ Properly terminate this program.
    mov r7, #1                  @ System call SYS_EXIT.
    swi 0                       @ Call software interrupt.

.data
message:
    .asciz "Hello World!\n"     @ Null terminated string.
len = .-message                 @ Length of string.

