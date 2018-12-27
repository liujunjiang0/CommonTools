// meminfo.cpp

#include <meminfo.h>

#include <sys/sysinfo.h>

namespace cts
{

meminfo::meminfo() : _total(0),
            _uptime(0)
{
    struct sysinfo info;
    
    if(sysinfo(&info) == 0)
    {
        _total = info.totalram;
        _uptime = info.uptime;
    }
}

std::string meminfo::usage() const
{
    size_t res = 0;
    struct sysinfo info;

    if(sysinfo(&info) == 0)
    {
        res = 100 - 100 * info.freeram / info.totalram;
    }

    return std::to_string(res).append("%");
}

size_t meminfo::uptime() const
{
    return _uptime;
}

} //namespace cts
