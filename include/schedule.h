// schedule.h

#ifndef _SCHEDULE_H_
#define _SCHEDULE_H

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <config.h>

CTS_BEGIN

class schedule
{
public:
    class task
    {
    public:
        task();

        void cancel();
        bool operator<(const task& lfs) const;
        
    private:
        struct _attribute_t;

        // For one time task, interval determines time delay to run. 0 means running immediately.
        // For persist task, it cannot run immediately.
        // seconds
        task(const std::function<void()>& func, std::uint64_t interval = 0, bool persist = false);

        std::uint64_t _delay();
        void _reset();

        std::shared_ptr<_attribute_t> _attr_ptr;

        friend class schedule;
    };

    schedule();
    ~schedule();

    task add_task(const std::function<void()>& func, std::uint64_t interval, bool persist = false);
    void pause();
    void run();

private:
    void _thread_func();

    std::atomic<bool> _canceled;
    std::atomic<bool> _paused;
    std::condition_variable _cv;
    std::mutex _cv_mtx;
    std::mutex _op_mtx;
    std::priority_queue<task> _task_queue;
    std::thread _thread;
};

CTS_END

#endif //_SCHEDULE_H__
