@ Created by Zhigang An on Mar. 10, 2017.
@ Case Western Reserve University
@ 
@ Required library: wiringPi
@ 
@ Assemble and link: gcc Blink_wiring.s -lwiringPi -o Blink_wiring
@ Run: ./Blink_wiring
@
@ Blink LED 10 times using wiringPi library.

.text
.global main
.extern printf
.extern wiringPiSetup
.extern delay
.extern digitalWrite
.extern pinMode

main:
    @ push return address and register for alignment.
    push {ip, lr}

    @ Setup GPIO and check if it fails.
    bl  wiringPiSetupGpio
    mov r1, #-1
    cmp r0, r1
    bne init
    ldr r0, =error
    bl  printf
    b   done

init:
    @ Configure pinmode as output.
    ldr r0, =pin
    ldr r0, [r0]
    mov r1, #1
    bl  pinMode

    @ Load init value to loop counter.
    mov r4, #1
    @ldr r4, [r4]
    @ Loop 10 times.
    mov r5, #10

loop:
    cmp r4, r5
    bgt done

    ldr r0, =pin
    ldr r0, [r0]
    mov r1, #HIGH
    bl  digitalWrite

    ldr r0, =delayInMs
    ldr r0, [r0]
    bl  delay

    ldr r0, =pin
    ldr r0, [r0]
    mov r1, #LOW
    bl  digitalWrite

    ldr r0, =delayInMs
    ldr r0, [r0]
    bl  delay

    @ Increment counter and loop.
    add r4, #1
    b   loop

done:
    @ Pop return address into pc.
    pop {ip, pc}


.data
.balign 4
error:
    .asciz "Failed to setup gpio.\n"
pin:
    @ Use GPIO16.
    .int 16
delayInMs:
    .int 200
HIGH = 1
LOW = 0
