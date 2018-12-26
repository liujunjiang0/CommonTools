// cpuinfo_test.cpp

// g++ -g -std=c++11 -I../include ../src/cpuinfo.cpp cpuinfo_test.cpp -o cpuinfo_test

#include <cpuinfo.h>

#include <algorithm>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    cts::cpuinfo ci;

    std::cout << "processors = " << ci.processors() << std::endl
        << "physical_processors = " << ci.physical_processors() << std::endl
        << "verdor = " << ci.vendor() << std::endl
        << "name = " << ci.name() << std::endl
        << "frequency = " << ci.frequency() << std::endl
        << "cache_size = " << ci.cache_size() << std::endl;

    cts::cpuinfo::flags_type flags = ci.flags();
    for_each(flags.cbegin(), flags.cend(), [](const std::string& flag)
    {
        std::cout << flag << std::endl;
    });

    std::cout << "CPU usage = " << ci.usage() << std::endl;

    std::ifstream ifs("/proc/stat");
    if(!ifs)
    {
        std::cout << "Open Error" << std::endl;
    }
    else
    {
        std::cout << "OK" << std::endl;
    }

    return 0;
}
