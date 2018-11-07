#include <iostream>
#include <thread>
#include <unistd.h>

#include <msgqueue.h>

#define MQ  "myqueue"

int main(int argc, char* argv[])
{
    cts::msgqueue::resource_limit(819200, 819200);

    cts::msgqueue mq_sender(MQ, true, 1280, 10);
    cts::msgqueue mq_receiver(MQ, false);
    
    std::cout << std::boolalpha << mq_sender.valid() << std::endl
            << mq_receiver.valid() << std::endl;

    std::thread([&]()
    {
        while(1)
        {
            sleep(5);
            std::string res = mq_receiver.receive(10000000);
            if (!res.empty())
            {
                std::cout << res << std::endl;
            }
            std::cout << mq_receiver.name() << std::endl
                    << mq_receiver.msgsize() << std::endl
                    << mq_receiver.capacity() << std::endl
                    << mq_receiver.count() << std::endl;
        }
    }).detach();

    int i = 10;
    std::string msg("Hello World");
    while(i--)
    {
        std::cout << std::boolalpha << mq_sender.send(msg, 1000000) << std::endl;
        sleep(1);
    }

    return 0;
}
