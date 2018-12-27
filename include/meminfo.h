// meminfo.h

#ifndef _MEMINFO_H_
#define _MEMINFO_H_

#include <string>

namespace cts
{

class meminfo
{
public:
    meminfo();
    ~meminfo() = default;

    // n%
    std::string usage() const;
    size_t uptime() const;   //seconds from boot

private:
    size_t _total;
    size_t _uptime;
};

} //namespace cts

#endif //_MEMINFO_H_
