#include "autofocus.h"

#include <iostream>
#include <chrono>
#include <string>

extern "C" void start_thread();
extern "C" void stop_thread();
extern "C" int get_counter();

using namespace autofocus;

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    char getch() {
        struct termios oldt, newt;
        char c;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        read(STDIN_FILENO, &c, 1);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }
#endif

int main() 
{
    std::cout.setf(std::ios::unitbuf);
    std::cout << "Press following keys :\n";
    std::cout << "  - \'q\' for quitting \n";

    //start_thread();

    while (true) 
    {
        char c;
        #ifdef _WIN32
            c = _getch();
        #else
            c = getch();
        #endif

        if (c == 'q') break;
    }

    //stop_thread();

    return 0;
}