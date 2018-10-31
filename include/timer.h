// timer.h

#ifndef _TIMER_H_
#define _TIMER_H_

#include <cstdint>
#include <functional>

#include <config.h>
#include <singleton.h>

CTS_BEGIN

//forward declare
void sig_callback(int sig);

class systimer : public singleton<systimer> 
{
public:
    /*
	@interval - ns, period of timer. If 0, it is a once timer.
	@start - ns, define the time for first timeout. If 0, it will use @interval.
	If both @interval and @start are 0, it is same as cancel.
    **/
    void run(std::uint64_t interval, std::function<void(int)> callback, std::uint64_t start = 0);
    void cancel();

private:
    systimer() = default;
    std::function<void(int)> _callback;
    
    friend void sig_callback(int sig);
    friend class singleton;
};

CTS_END

#endif //_TIMER_H_
