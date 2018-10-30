// singleton.h

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <mutex>
#include <memory>

#include <config.h>
#include <nonecopyable.h>

CTS_BEGIN

/*
 friend class singleton; MUST be included in the derived class.
 The derived class only has default constructor with private access.
**/
template <typename T>
class singleton : public nonecopyable
{
public:
	static std::shared_ptr<T> instance()
	{
		if(_obj.get() == nullptr)
		{
			std::lock_guard<std::mutex> lck(_mtx);
			if(_obj.get() == nullptr)
			{
				_obj.reset(new T);
			}
		}
		return _obj;
	}
protected:
	singleton(){}
private:
	static std::shared_ptr<T> _obj;
	static std::mutex _mtx;
};

template <typename T>
std::shared_ptr<T> singleton<T>::_obj;
template <typename T>
std::mutex singleton<T>::_mtx;

CTS_END

#endif //_SINGLETON_H_
