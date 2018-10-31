// timer.cpp

#include <csignal>
#include <sys/time.h>

#include <timer.h>

CTS_BEGIN

void systimer::run(std::uint64_t interval, std::function<void(int)> callback, std::uint64_t start)
{
    _callback = callback;
    //setup callback, acctually catch the SIGALRM signal.
    signal(SIGALRM, sig_callback);

    struct itimerval t;
    if(start == 0)
    {
        t.it_value.tv_sec = interval/1000000;
	t.it_value.tv_usec = interval%1000000;
    }
    else
    {
	t.it_value.tv_sec = start/1000000;
	t.it_value.tv_usec = start%1000000;
    }
    t.it_interval.tv_sec = interval/1000000;
    t.it_interval.tv_usec = interval%1000000;

    setitimer(ITIMER_REAL, &t, nullptr);
}

void systimer::cancel()
{
    struct itimerval t {{0, 0}, {0, 0}};
    setitimer(ITIMER_REAL, &t, nullptr);
}

void sig_callback(int sig)
{
    systimer::instance()->_callback(sig);    
}

CTS_END
