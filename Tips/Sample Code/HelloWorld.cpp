// Create by AnZhg on Jan. 3, 2017.
// Case Western Reserve University
//
// Sample code for compiling.
// Compile: g++ -Wall -Wextra -Werror -Os HelloWorld.cpp -o HelloWorld
// Run: ./HelloWorld

#include <iostream>

int main(int argc, const char *argv[])
{
    // Supress compiler warning: unused variable(s).
    (void)argc;
    (void)argv;
    
    std::cout << "Hello World!" << std::endl;
    
    return 0;
}
