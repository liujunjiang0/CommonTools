// msgqueue.h

#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <config.h>

CTS_BEGIN

class msgqueue
{
public:
    msgqueue(const std::string& name, bool create, std::uint32_t msgsize = 8 * 1024, std::uint32_t capacity = 10);
    ~msgqueue();

    bool valid() const;
    std::string name() const;
    std::uint32_t msgsize() const;
    std::uint32_t capacity() const;
    std::uint32_t count() const;

    bool send(const std::string& buf, std::uint32_t timeout = 0);
    std::string receive(std::uint32_t timeout = 0);

    static void resource_limit(const std::uint32_t soft = 8 * 1024 * 1024, const std::uint32_t hard = 8 * 1024 * 1024);

private:
    struct handle_t;

    void _open();
    void _close();

    bool _create;
    int _errno;
    std::string _fullname;
    std::string _name;
    std::uint32_t _capacity;
    std::uint32_t _size;
    std::unique_ptr<handle_t> _fd;
};

CTS_END

#endif //_MSGQUEUE_H_
