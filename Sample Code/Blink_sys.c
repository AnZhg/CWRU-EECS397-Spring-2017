// Create by AnZhg on Jan. 14, 2017.
// Case Western Reserve University
//
// Compile: gcc -Wall -Wextra -Werror -Os Blink_sys.c -o Blink_sys
// Run: sudo ./Blink_sys
// Using system commands to control RGB LED.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    
    // Make these GPIO's available.
    system("echo \"17\" > /sys/class/gpio/export");    // Red.
    system("echo \"27\" > /sys/class/gpio/export");    // Green.
    system("echo \"22\" > /sys/class/gpio/export");    // Blue.
    
    // Set direction.
    system("echo out > /sys/class/gpio/gpio17/direction");
    system("echo out > /sys/class/gpio/gpio27/direction");
    system("echo out > /sys/class/gpio/gpio22/direction");
    
    // Red.
    // Turn on.
    system("echo 1 > /sys/class/gpio/gpio17/value");
    
    // Delay around 2 seconds.
    system("sleep 2");
    
    // Turn off.
    system("echo 0 > /sys/class/gpio/gpio17/value");
    
    // Green.
    // Turn on.
    system("echo 1 > /sys/class/gpio/gpio27/value");
    
    // Delay around 2 seconds.
    system("sleep 2");
    
    // Turn off.
    system("echo 0 > /sys/class/gpio/gpio27/value");
    
    // Blue.
    // Turn on.
    system("echo 1 > /sys/class/gpio/gpio22/value");
    
    // Delay around 2 seconds.
    system("sleep 2");
    
    // Turn off.
    system("echo 0 > /sys/class/gpio/gpio22/value");
    
    // Give GPIO resources back to operating system.
    system("echo \"17\" > /sys/class/gpio/unexport");
    system("echo \"27\" > /sys/class/gpio/unexport");
    system("echo \"22\" > /sys/class/gpio/unexport");
    
    return 0;
}
