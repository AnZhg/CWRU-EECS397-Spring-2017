// Create by Zhigang An on Jan. 3, 2017.
// Case Western Reserve University
//
// Sample code for compiling.
// Compile: g++ -Wall -Wextra -Werror -Os HelloWorld.cpp -o HelloWorld_cpp
// Run: ./HelloWorld_cpp

#include <iostream>

// C++ class declaration.
class HelloWorld
{
    public:
        HelloWorld();
        HelloWorld(const HelloWorld& copy_from);
        HelloWorld& operator=(const HelloWorld& copy_from);

        void printMsg();

    private:
        std::string world;
};

// Constructor.
HelloWorld::HelloWorld(void)
{
    world = "World";
}

// Copy constructor.
HelloWorld::HelloWorld(const HelloWorld& copy_from)
{
   world = copy_from.world;
}

// Copy assignment.
HelloWorld& HelloWorld::operator=(const HelloWorld& copy_from)
{
   world = copy_from.world;
}

void HelloWorld::printMsg()
{
   std::cout << "Hello " + world + "!" << std::endl;
}

// main.
int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    HelloWorld hello;

    hello.printMsg();
    
    return 0;
}
