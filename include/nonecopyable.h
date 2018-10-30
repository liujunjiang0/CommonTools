// nonecopyable.h

#ifndef _NONECOPYABLE_H_
#define _NONECOPYABLE_H_

#include <config.h>

CTS_BEGIN

class nonecopyable
{
protected:
    nonecopyable() {};
    nonecopyable(const nonecopyable&) = delete;
    nonecopyable& operator=(const nonecopyable&) = delete;
};

CTS_END

#endif //_NONECOPYABLE_H_
