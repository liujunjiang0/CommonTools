// schedule.cpp

#include <ctime>
#include <iostream>

#include <schedule.h>

CTS_BEGIN

struct schedule::task::_attribute_t
{
    bool persist = false;
    std::atomic<bool> canceled {true};
    std::function<void()> func;
    std::uint64_t interval = 0;
    std::uint64_t the_time = 0;
};

schedule::task::task() : 
                    _attr_ptr(new _attribute_t)
{

}

void schedule::task::cancel()
{
    _attr_ptr->canceled = true;
}

bool schedule::task::operator<(const task& lfs) const
{
    // priority_queue's first element is always the greatest. Here it needs the smallest.
   return  _attr_ptr->the_time > lfs._attr_ptr->the_time;
}

schedule::task::task(const std::function<void()>& func, std::uint64_t interval, bool persist) :
                _attr_ptr(new _attribute_t)
{
    _attr_ptr->func = func;
    _attr_ptr->interval = interval;
    _attr_ptr->the_time = interval + time(0);
    _attr_ptr->persist = persist;
    _attr_ptr->canceled = false;
}

void schedule::task::_reset()
{
    if (_attr_ptr->persist)
    {
        _attr_ptr->the_time = _attr_ptr->interval + time(0);
    }
}

std::uint64_t schedule::task::_delay()
{
    std::uint64_t cur = time(0);
    
    if (_attr_ptr->the_time < cur)
    {
        return 0;
    }

    return _attr_ptr->the_time - cur;
}

schedule::schedule() :
            _canceled(false),
            _paused(false)
{
    std::thread(std::bind(&schedule::_thread_func, this)).detach();
}

schedule::~schedule()
{
    _canceled = true;
}

schedule::task schedule::add_task(const std::function<void()>& func, std::uint64_t interval, bool persist)
{
    task t(func, interval, persist);
    std::lock_guard<std::mutex> lck(_op_mtx);
    _task_queue.push(t);
    _cv.notify_one();

    return t;
}

void schedule::pause()
{
    _paused = true;
}

void schedule::run()
{
    _paused = false;

    _cv.notify_one();
}

void schedule::_thread_func()
{
    while (!_canceled)
    {
        if (_paused)
        {
            std::unique_lock<std::mutex> lck(_cv_mtx);
            std::cout << "paused" << std::endl;
            _cv.wait(lck);

            std::cout << "run" << std::endl;
            continue;
        }

        _op_mtx.lock();

        while ((!_task_queue.empty()) && (_task_queue.top()._attr_ptr->canceled))
        {
            _task_queue.pop();
        }

        if (_task_queue.empty())
        {
            _op_mtx.unlock();
            
            std::unique_lock<std::mutex> lck(_cv_mtx);
            _cv.wait(lck);

            continue;
        }

        task t(_task_queue.top());
        std::uint64_t delay = t._delay();
        if (delay > 0)
        {
            _op_mtx.unlock();

            std::unique_lock<std::mutex> lck(_cv_mtx);
            _cv.wait_for(lck, std::chrono::seconds(delay));

            continue;
        }

        _task_queue.pop();
        if (t._attr_ptr->persist)
        {
            t._reset();
            _task_queue.push(t);
        }

        _op_mtx.unlock();

        t._attr_ptr->func();
    }
}

CTS_END
