// Create by Zhigang An on Jan. 3, 2017.
// Case Western Reserve University
//
// Sample code for compiling.
// Compile: gcc -Wall -Wextra -Werror -Os HelloWorld.c -o HelloWorld_c
// Run: ./HelloWorld_c

#include <stdio.h>

int main(int argc, const char *argv[])
{
    // Supress compiler warning: unused variable(s).
    (void)argc;
    (void)argv;
    
    printf("Hello World!\n");
    
    return 0;
}
