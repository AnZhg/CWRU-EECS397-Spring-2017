// Create by AnZhg on Jan. 14, 2017.
// Case Western Reserve University
//
// Compile: gcc -Wall -Wextra -Werror -Os NAME.c -o NAME

#include <stdio.h>
#include<stdlib.h>

int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    
    // Make these GPIO's available.
    system("gpio-admin export 17");    // Red.
    system("gpio-admin export 27");    // Green.
    system("gpio-admin export 22");    // Blue.
    
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
    system("gpio-admin unexport 17");
    system("gpio-admin unexport 27");
    system("gpio-admin unexport 22");
    
    return 0;
}
