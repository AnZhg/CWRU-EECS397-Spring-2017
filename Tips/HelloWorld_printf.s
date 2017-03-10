@ Created by Zhigang An on Mar. 10, 2017.
@ Case Western Reserve University
@ 
@ Assemble and link: gcc HelloWorld_printf.s -o HelloWorld_printf
@ Run: ./HelloWorld_printf
@ 
@ Another way to print string in stdout (using printf function).

.text
.global main
.extern printf

main:
    push {ip, lr}
    
    @ First parameter of function is passed by register r0.
    ldr  r0, =string
    @ Make function call.
    bl   printf
    
    pop {ip, pc}

.data
string:
    .asciz "Hello World!\n"

