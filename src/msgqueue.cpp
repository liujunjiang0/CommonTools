// msgqueue.cpp

#include <fcntl.h>
#include <mqueue.h>
#include <sys/resource.h>
#include <sys/stat.h>

#include <msgqueue.h>

CTS_BEGIN

struct msgqueue::handle_t
{
    mqd_t mqd;
};

msgqueue::msgqueue(const std::string& name, bool create, std::uint32_t msgsize, std::uint32_t capacity) : 
            _create(create),
            _fd(new handle_t),
            _name(name),
            _capacity(capacity),
            _size(msgsize)

{
    _fd->mqd = -1;
    _errno = -1;
    _fullname = "/" + _name;

    _open();
}

msgqueue::~msgqueue()
{
    _close();
}

bool msgqueue::valid() const
{
    return 0 == _errno;
}

std::string msgqueue::name() const
{
    return _name;
}

std::uint32_t msgqueue::msgsize() const
{
    return _size;
}

std::uint32_t msgqueue::capacity() const
{
    return _capacity;
}

std::uint32_t msgqueue::count() const
{
    int count = 0;

    if (valid())
    {
        mq_attr attr = {0};
        mq_getattr(_fd->mqd, &attr);
        count = attr.mq_curmsgs;
    }

    return count;
}

// ns
bool msgqueue::send(const std::string& buf, std::uint32_t timeout)
{
    if (!valid())
    {
        return false;
    }

    timespec ts = {0};
    ts.tv_sec = timeout / 1000000;
    ts.tv_nsec = timeout % 1000000;

    if (mq_timedsend(_fd->mqd, buf.c_str(), buf.length() + 1, 0, &ts) != 0)
    {
        return false;
    }

    return true;
}

std::string msgqueue::receive(std::uint32_t timeout)
{
    if (!valid())
    {
        return std::string(); 
    }

    int bytes = -1;
    std::string buf(_size, '\0');
    timespec ts = {0};
    ts.tv_sec = timeout / 1000000;
    ts.tv_nsec = timeout % 1000000;

    bytes = mq_timedreceive(_fd->mqd, &buf[0], buf.length(), nullptr, &ts);
    if (bytes < 0)
    {
        bytes = 0;
    }
    buf.resize(bytes);

    return buf;
}

void msgqueue::_open()
{
    mq_attr attr = {0};

    if (_create)
    {
        attr.mq_maxmsg = _capacity;
        attr.mq_msgsize = _size;
        _fd->mqd = mq_open(_fullname.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, &attr);
    }
    else
    {
        _fd->mqd = mq_open(_fullname.c_str(), O_RDWR);
    }

    if (_fd->mqd != -1)
    {
        if (mq_getattr(_fd->mqd, &attr) == 0)
        {
            _capacity = attr.mq_maxmsg;
            _size = attr.mq_msgsize;
            _errno = 0;
        }
        else
        {
            _errno = errno;
        }
    }
    else
    {
        _errno = errno;
    }
}

void msgqueue::_close()
{
    mq_close(_fd->mqd);
    _fd->mqd = -1;
    if (_create)
    {
        mq_unlink(_fullname.c_str());
    }
    _errno = -1;
}

void msgqueue::resource_limit(const std::uint32_t soft, const std::uint32_t hard)
{
    rlimit rlmt = {soft, hard};
    setrlimit(RLIMIT_MSGQUEUE, &rlmt);
}

CTS_END
