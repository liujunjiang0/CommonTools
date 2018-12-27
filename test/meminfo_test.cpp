// meminfo_test.cpp

// g++ -g -std=c++11 -I../include ../src/meminfo.cpp meminfo_test.cpp -o meminfo_test

#include <meminfo.h>

#include <iostream>

int main(int argc, char* argv[])
{
    cts::meminfo mi;

    std::cout << "Uptime = " << mi.uptime() << std::endl
            << "Usage = " << mi.usage() << std::endl;

    return 0;
}
