// system_test.cpp

// g++ -std=c++11 -I../include ../src/system.cpp system_test.cpp -o system_test

#include <system.h>

int main()
{
    //system::reboot();
    system::shutdown();

    return 0;
}
