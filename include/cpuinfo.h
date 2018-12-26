// cpuinfo.h

#ifndef _CPUINFO_H_
#define _CPUINFO_H_

#include <set>
#include <string>
#include <vector>

namespace cts
{

class cpuinfo
{
public:
    typedef std::set<std::string> flags_type;

    typedef struct info
    {
        int processor_id;
        int physical_id;
        int cores;
        int core_id;
        std::string vendor;
        std::string name;
        std::string frequency;
        std::string cache_size;
        flags_type flags;

        void clear();
    }info_t, *info_ptr;

    cpuinfo();
    ~cpuinfo() = default;

    // Query static infomations.
    size_t processors() const;
    size_t physical_processors() const;
    std::string vendor(size_t index = 0) const;
    std::string name(size_t index = 0) const;
    std::string frequency(size_t index = 0) const;
    std::string cache_size(size_t index = 0) const;
    flags_type flags(size_t index = 0) const;

    // cpu usage
    std::string usage();
    void interval(int i);

private:
    typedef struct cpu_time
    {
        int user;
        int system;
        int idle;
        int nice;

        void clear();
    }cpu_time_t, *cpu_time_ptr;

    std::string _value(const std::string& l);
    int _time(const std::string& info, size_t& start, size_t& pos);
    void _cpu_time(cpu_time_t& t);

    cpu_time_t _first;
    cpu_time_t _second;
    int _interval;     // s, because ms cannot calculate the usage
    std::vector<info_t> _infos;
};

} //namespace cts

#endif //_CPUINFO_H_
