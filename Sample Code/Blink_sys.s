@ Created by Zhigang An on Mar. 10, 2017.
@ Case Western Reserve University
@ 
@ Assemble and link: gcc Blink_sys.s -o Blink_sys
@ Run: sudo ./Blink_sys
@
@ Blink LED 10 times using system commands.

.text
.global main
.extern sprintf
.extern system

main:
    @ push return address and register for alignment.
    push {ip, lr}

    @ Export GPIO.
    @ Load the first parameter in register r0.
    @ Destination string of sprintf.
    ldr  r0, =cmd_string
    @ Load the second parameter in register r1.
    @ Format string of sprintf.
    ldr  r1, =export_cmd
    @ Load the third parameter in register r2.
    @ Format parmater of sprintf.
    ldr  r2, =pin
    ldr  r2, [r2]
    @ Make function call sprintf
    bl   sprintf

    @ Load full command.
    ldr  r0, =cmd_string
    @ Make function call to system.
    bl   system
    
    
    @ Configure mode as out.
    ldr  r0, =cmd_string
    ldr  r1, =out_cmd
    ldr  r2, =pin
    ldr  r2, [r2]
    bl   sprintf
    
    ldr  r0, =cmd_string
    bl   system
    
    
    @ Load init value to loop counter.
    mov r4, #1
    @ Loop 10 times.
    mov r5, #10
    
    
loop:
    @ Check loop condition.
    cmp  r4, r5
    bgt  done
    
    
    @ Pull high.
    ldr  r0, =cmd_string
    ldr  r1, =high_cmd
    ldr  r2, =pin
    ldr  r2, [r2]
    bl   sprintf
    
    ldr  r0, =cmd_string
    bl   system
    
    
    @ Delay 1 s.
    ldr  r0, =delay_cmd
    bl   system
    
    
    @ Pull low.
    ldr  r0, =cmd_string
    ldr  r1, =low_cmd
    ldr  r2, =pin
    ldr  r2, [r2]
    bl   sprintf
    
    ldr  r0, =cmd_string
    bl   system
    
    
    @ Delay 1 s.
    ldr  r0, =delay_cmd
    bl   system
    
    
    @ Increment counter and loop.
    add  r4, #1
    b    loop
    
    
    @ Unexport GPIO.
    ldr  r0, =cmd_string
    ldr  r1, =unexport_cmd
    ldr  r2, =pin
    ldr  r2, [r2]
    bl   sprintf
    
    ldr  r0, =cmd_string
    bl   system
    
    
done:
    @ Pop return address into pc.
    pop {ip, pc}
    
.data
.balign 4

cmd_string:
    @ Blank string with space of 44 bytes.
    .space 44
export_cmd:
    .asciz "echo \"%i\" > /sys/class/gpio/export"
unexport_cmd:
    .asciz "echo \"%i\" > /sys/class/gpio/unexport"
out_cmd:
    .asciz "echo out > /sys/class/gpio/gpio%i/direction"
high_cmd:
    .asciz "echo 1 > /sys/class/gpio/gpio%i/value"
low_cmd:
    .asciz "echo 0 > /sys/class/gpio/gpio%i/value"
delay_cmd:
    .asciz "sleep 1"

pin:
    @ Use GPIO16.
    .int 16

