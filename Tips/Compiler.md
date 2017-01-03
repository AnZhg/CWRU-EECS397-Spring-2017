**C/C++**

The C/C++ compiler comes with Raspberry Pi (Raspbian version 4.4.34) is GNU Compiler Collection. To compile a C file (HelloWorld.c as an example), run the following command:

    gcc -Wall -Werror -Os HelloWorld.c -o HelloWorld

The `-Wall` flag enables all warnings. `-Werror` forces the compiler to treat warnings as errors. With `-Os` flag, all debug symbols will be stripped, and compiler will optimize the generated executable (smallest, fastest). Debug symbols are useful for debugging the code, but when using the executable instead of debugging/testing, optimized version is prefered ("cleaner", faster).

After running the command, an executable named HelloWorld should appear if there is no error.

GCC also supports: Objective-C, Objective-C++, and GO programming languages.

More Infomation see [this link](https://gcc/gnu/org/onlinedocs/gcc/).

<br>
**Java**
Java (Version 1.8.0) is pre-installed in Raspbian. There is also GUI IDE in Raspbian Menu - Programming - BlueJ Java IDE.

<br>
**Python**
If you love using python, python 2.7.9 and python 3.4.2 come with Raspbian. there are also GUI python IDE, you can find them in Raspberry Menu - Programming - Python 2 (IDLE) / Python 3 (IDLE).

<br>
**Your Preference**
You can also install your favoriote compiler, such as clang. Run the following command in a terminal:

    sudo apt-get install clang

Clang has a similar syntax when compiling C/C++ source code:

For C code:

    clang -Wall -Werror -Os HelloWorld.c -o HelloWorld

For C++ code:

    clang++ -Wall -Werror -Os HelloWorld.cpp -o HelloWorld

