// systimer_test.cpp

#include <iostream>
#include <thread>
#include <unistd.h>

#include <timer.h>

int main()
{
    // Calling thread will weakup if it is sleeping when systimer is timeout.
    // --> DO NOT influence other threads.
    std::thread([]()
    {
	std::cout << "Thread running." << std::endl;
	sleep(10);
	std::cout << "Thread runout." << std::endl;
    }).detach();

    // period timer.
    cts::systimer::instance()->run(1000000, [](int sig)
    {
	std::cout << sig << std::endl;
    });

    int i = 5;
    while(i--)
	sleep(10000);
    
    cts::systimer::instance()->cancel();

    // once timer.
    cts::systimer::instance()->run(0, [](int sig)
    {
        std::cout << sig << std::endl;
    }, 3000000);

    while(1)
	sleep(10000);

    return 0;
}
