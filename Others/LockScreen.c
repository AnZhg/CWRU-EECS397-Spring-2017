// Create by AnZhg on Jan. 3, 2017.
// Case Western Reserve University
//
// Sample code for compiling.
// Compile: gcc -Wall -Wextra -Werror -Os LockScreen.c -o Lock\ Screen
// Run: ./Lock\ Screen
// Prerequisite: xscreensaver
// Install: sudo apt-get install xscreensaver

#include <stdlib.h>

int main(int argc, const char *argv[])
{
    // Supress compiler warning: unused variable(s).
    (void)argc;
    (void)argv;
    
    system("xscreensaver-command -lock");
    system("exit");
    
    return 0;
}
