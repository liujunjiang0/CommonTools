// cpuinfo.cpp

#include <cpuinfo.h>

#include <algorithm>
#include <fstream>
#include <unistd.h>

namespace cts
{

static const std::string INFO_FILE = "/proc/cpuinfo";
static const std::string USAGE_FILE = "/proc/stat";

static const std::string PROCESSOR = "processor";
static const std::string VENDOR = "vendor_id";
static const std::string NAME = "model name";
static const std::string FREQUENCY = "cpu MHz";
static const std::string CACHE_SIZE = "cache size";
static const std::string PHYSICAL_ID = "physical id";
static const std::string CORE_ID = "core id";
static const std::string CORES = "cpu cores";
static const std::string FLAGS = "flags";
static const char DELIM = ':';
static const char SPACE = ' ';

void cpuinfo::info::clear()
{
    processor_id = -1;
    physical_id = -1;
    cores = -1;
    core_id = -1;
    vendor.clear();
    name.clear();
    frequency.clear();
    cache_size.clear();
    flags.clear();
}

cpuinfo::cpuinfo() : _interval(1)
{
    std::string line;
    std::ifstream ifs(INFO_FILE);
    info_t info;

    if(!ifs)
    {
        return;
    }

    do
    {
        std::getline(ifs, line);

        if(ifs.eof())
        {
            break;
        }

        if(line.compare(0, PROCESSOR.length(), PROCESSOR) == 0)
        {
            info.clear();
            info.processor_id = std::stoi(_value(line));
        }
        else if(line.compare(0, VENDOR.length(), VENDOR) == 0)
        {
            info.vendor = _value(line);
        }
        else if(line.compare(0, NAME.length(), NAME) == 0)
        {
            info.name = _value(line);
        }
        else if(line.compare(0, FREQUENCY.length(), FREQUENCY) == 0)
        {
            info.frequency = _value(line).append(" MHz");
        }
        else if(line.compare(0, CACHE_SIZE.length(), CACHE_SIZE) == 0)
        {
            info.cache_size = _value(line);
        }
        else if(line.compare(0, PHYSICAL_ID.length(), PHYSICAL_ID) == 0)
        {
            info.physical_id = std::stoi(_value(line));
        }
        else if(line.compare(0, CORE_ID.length(), CORE_ID) == 0)
        {
            info.core_id = std::stoi(_value(line));
        }
        else if(line.compare(0, CORES.length(), CORES) == 0)
        {
            info.cores = std::stoi(_value(line));
        }
        else if(line.compare(0, FLAGS.length(), FLAGS) == 0)
        {
            size_t start = 0;
            size_t pos = 0;
            std::string collection = _value(line);

            do
            {
                pos = collection.find(SPACE, start);
                if(pos != std::string::npos)
                {
                    info.flags.insert(collection.substr(start, pos - start));
                    start = pos + 1;
                }
                else
                {
                    info.flags.insert(collection.substr(start));
                    break;
                }
            }while(true);

            _infos.push_back(info);
        }
    }while(true);

    ifs.close();
}

size_t cpuinfo::processors() const
{
    return _infos.size();
}

size_t cpuinfo::physical_processors() const
{
    std::set<int> counter;
    for_each(_infos.cbegin(), _infos.cend(), [&](const info_t& info)
    {
        counter.insert(info.physical_id);
    });

    return counter.size();
}

std::string cpuinfo::vendor(size_t index) const
{
    if(index < _infos.size())
    {
        return _infos[index].vendor;
    }

    return std::string();
}

std::string cpuinfo::name(size_t index) const
{
    if(index < _infos.size())
    {
        return _infos[index].name;
    }

    return std::string();
}

std::string cpuinfo::frequency(size_t index) const
{
    if(index < _infos.size())
    {
        return _infos[index].frequency;
    }

    return std::string();
}

std::string cpuinfo::cache_size(size_t index) const
{
    if(index < _infos.size())
    {
        return _infos[index].cache_size;
    }

    return std::string();
}

cpuinfo::flags_type cpuinfo::flags(size_t index) const
{
    if(index < _infos.size())
    {
        return _infos[index].flags;
    }

    return flags_type();
}

void cpuinfo::cpu_time::clear()
{
    user = 0;
    system = 0;
    idle = 0;
    nice = 0;
}

std::string cpuinfo::usage()
{
    _cpu_time(_first);
    sleep(_interval);
    _cpu_time(_second);

    int res = 0;
    int delta_user = _second.user - _first.user;
    int delta_system = _second.system - _first.system;
    int delta_total = (_second.user + _second.nice + _second.system + _second.idle) - (_first.user + _first.nice + _first.system + _first.idle);

    if(delta_total > 0)
    {
        res = (delta_user + delta_system) * 100 / delta_total;
    }

    return std::to_string(res).append("%");
}

void cpuinfo::interval(int i)
{
    _interval = i;
}

std::string cpuinfo::_value(const std::string& l)
{
    size_t pos = l.find(DELIM);
    if(pos != std::string::npos)
    {
        // Always return from here.
        return l.substr(pos + 2);
    }

    return std::string();
}

int cpuinfo::_time(const std::string& info, size_t& start, size_t& pos)
{
    int ret = 0;

    pos = info.find(SPACE, start);
    if(pos == std::string::npos)
    {
       return 0;
    }
    ret = std::stoi(info.substr(start, pos));
    start = pos + 1;

    return ret;
}

void cpuinfo::_cpu_time(cpu_time_t& t)
{
    t.clear();

    std::string info;
    std::ifstream ifs(USAGE_FILE);

    if(!ifs)
    {
        return;
    }

    std::getline(ifs, info);
    if(info.compare(0, 3, "cpu") == 0)
    {
        size_t start = info.find_first_of("0123456789", 3);
        size_t pos = 0;

        if(start == std::string::npos)
        {
            return;
        }

        t.user = _time(info, start, pos);
        t.nice = _time(info, start, pos);
        t.system = _time(info, start, pos);
        t.idle = _time(info, start, pos);
    }

    ifs.close();
}

}//namespace cts
