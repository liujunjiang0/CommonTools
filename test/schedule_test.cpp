// schedule_test.cpp

#include <functional>
#include <iostream>
#include <unistd.h>

#include <schedule.h>

void func()
{
    std::cout << "Hello World." << std::endl;
}

class test
{
public:
    test(cts::schedule& schd) : 
        _schd(schd)
    {
        _task = _schd.add_task(std::bind(&test::func, this, 5), 1, true);
    }

    ~test()
    {
        _task.cancel();
    }

    void func(int i)
    {
        std::cout << i << std::endl;
    }

private:
    cts::schedule& _schd;
    cts::schedule::task _task;
};

int main(int argc, char* argv[])
{
    cts::schedule schd;
    cts::schedule::task t1 = schd.add_task(std::bind(func), 1, true);
    sleep(5);
    t1.cancel();

    schd.add_task([]()
    {
        std::cout << "lambda" << std::endl;
    }, 1, true);
    sleep(5);
    schd.pause();
    sleep(5);
    schd.run();

    {
        test t(schd);
        sleep(5);
    }

    sleep(11111);

    return 0;
}
