// delegate.h

#ifndef _DELEGATE_H_
#define _DELEGATE_H_

#include <map>
#include <string>

#include <config.h>

CTS_BEGIN

// generic declaration, not defined.
template <typename Func>
class delegate;

// Specializations.
// For none parameter.
template <typename Ret>
class delegate<Ret()>
{
public:
	typedef Ret return_type;
	typedef std::function<return_type()> functor;

	// constructors
	delegate() {}
	delegate(std::string key, const functor& f)
	{
		add(key, f);
	}

	delegate(const delegate& dlg)
	{
		*this += dlg;
	}

	delegate& operator+=(const delegate& dlg)
	{
		// The key MUST be unique.
		_f_containor.insert(dlg._f_containor.begin(), dlg._f_containor.end());
		return *this;
	}

	delegate& add(const std::string& key, const functor& f)
	{
		_f_containor[key] = f;
		return *this;
	}

	delegate& cancel(const std::string& key)
	{
		auto it = _f_containor.find(key);
		if (it != _f_containor.end())
		{
			_f_containor.erase(it);
		}
		return *this;
	}

	delegate& operator-=(const delegate& dlg)
	{
		if (this == &dlg)
		{
			_f_containor.clear();
		}
		else
		{
			for (auto it = dlg._f_containor.cbegin(); it != dlg._f_containor.cend(); it++)
			{
				cancel(it->first);
			}
		}
		return *this;
	}

	// Only return the first functor's return.
	return_type operator()()
	{
		if (_f_containor.empty())
		{
			return return_type();
		}

		for (auto it = ++_f_containor.cbegin(); it != _f_containor.cend(); it++)
		{
			it->second();
		}

		return _f_containor.cbegin()->second();
	}

private:
	std::map<std::string, functor> _f_containor;
};

// For 1 parameter.
template <typename Ret, typename P1>
class delegate<Ret(P1)>
{
public:
	typedef Ret return_type;
	typedef P1  param1_type;
	typedef std::function<return_type(param1_type)> functor;

	// constructors
	delegate() {}
	delegate(std::string key, const functor& f)
	{
		add(key, f);
	}

	delegate(const delegate& dlg)
	{
		*this += dlg;
	}

	delegate& operator+=(const delegate& dlg)
	{
		// The key MUST be unique.
		_f_containor.insert(dlg._f_containor.begin(), dlg._f_containor.end());
		return *this;
	}

	delegate& add(const std::string& key, const functor& f)
	{
		_f_containor[key] = f;
		return *this;
	}

	delegate& cancel(const std::string& key)
	{
		auto it = _f_containor.find(key);
		if (it != _f_containor.end())
		{
			_f_containor.erase(it);
		}
		return *this;
	}

	delegate& operator-=(const delegate& dlg)
	{
		if (this == &dlg)
		{
			_f_containor.clear();
		}
		else
		{
			for (auto it = dlg._f_containor.cbegin(); it != dlg._f_containor.cend(); it++)
			{
				cancel(it->first);
			}
		}
		return *this;
	}

	// Only return the first functor's return.
	return_type operator()(param1_type p1)
	{
		if (_f_containor.empty())
		{
			return return_type();
		}

		for (auto it = ++_f_containor.cbegin(); it != _f_containor.cend(); it++)
		{
			it->second(p1);
		}

		return _f_containor.cbegin()->second(p1);
	}

private:
	std::map<std::string, functor> _f_containor;
};

// For 2 parameter.
template <typename Ret, typename P1, typename P2>
class delegate<Ret(P1, P2)>
{
public:
	typedef Ret return_type;
	typedef P1  param1_type;
	typedef P2  param2_type;
	typedef std::function<return_type(param1_type, param2_type)> functor;

	// constructors
	delegate() {}
	delegate(std::string key, const functor& f)
	{
		add(key, f);
	}

	delegate(const delegate& dlg)
	{
		*this += dlg;
	}

	delegate& operator+=(const delegate& dlg)
	{
		// The key MUST be unique.
		_f_containor.insert(dlg._f_containor.begin(), dlg._f_containor.end());
		return *this;
	}

	delegate& add(const std::string& key, const functor& f)
	{
		_f_containor[key] = f;
		return *this;
	}

	delegate& cancel(const std::string& key)
	{
		auto it = _f_containor.find(key);
		if (it != _f_containor.end())
		{
			_f_containor.erase(it);
		}
		return *this;
	}

	delegate& operator-=(const delegate& dlg)
	{
		if (this == &dlg)
		{
			_f_containor.clear();
		}
		else
		{
			for (auto it = dlg._f_containor.cbegin(); it != dlg._f_containor.cend(); it++)
			{
				cancel(it->first);
			}
		}
		return *this;
	}

	// Only return the first functor's return.
	return_type operator()(param1_type p1, param2_type p2)
	{
		if (_f_containor.empty())
		{
			return return_type();
		}

		for (auto it = ++_f_containor.cbegin(); it != _f_containor.cend(); it++)
		{
			it->second(p1, p2);
		}

		return _f_containor.cbegin()->second(p1, p2);
	}

private:
	std::map<std::string, functor> _f_containor;
};

// For 3 parameter.
template <typename Ret, typename P1, typename P2, typename P3>
class delegate<Ret(P1, P2, P3)>
{
public:
	typedef Ret return_type;
	typedef P1  param1_type;
	typedef P2  param2_type;
	typedef P3  param3_type;
	typedef std::function<return_type(param1_type, param2_type, param3_type)> functor;

	// constructors
	delegate() {}
	delegate(std::string key, const functor& f)
	{
		add(key, f);
	}

	delegate(const delegate& dlg)
	{
		*this += dlg;
	}

	delegate& operator+=(const delegate& dlg)
	{
		// The key MUST be unique.
		_f_containor.insert(dlg._f_containor.begin(), dlg._f_containor.end());
		return *this;
	}

	delegate& add(const std::string& key, const functor& f)
	{
		_f_containor[key] = f;
		return *this;
	}

	delegate& cancel(const std::string& key)
	{
		auto it = _f_containor.find(key);
		if (it != _f_containor.end())
		{
			_f_containor.erase(it);
		}
		return *this;
	}

	delegate& operator-=(const delegate& dlg)
	{
		if (this == &dlg)
		{
			_f_containor.clear();
		}
		else
		{
			for (auto it = dlg._f_containor.cbegin(); it != dlg._f_containor.cend(); it++)
			{
				cancel(it->first);
			}
		}
		return *this;
	}

	// Only return the first functor's return.
	return_type operator()(param1_type p1, param2_type p2, param3_type p3)
	{
		if (_f_containor.empty())
		{
			return return_type();
		}

		for (auto it = ++_f_containor.cbegin(); it != _f_containor.cend(); it++)
		{
			it->second(p1, p2, p3);
		}

		return _f_containor.cbegin()->second(p1, p2, p3);
	}

private:
	std::map<std::string, functor> _f_containor;
};

// For 4 parameter.
template <typename Ret, typename P1, typename P2, typename P3, typename P4>
class delegate<Ret(P1, P2, P3, P4)>
{
public:
	typedef Ret return_type;
	typedef P1  param1_type;
	typedef P2  param2_type;
	typedef P3  param3_type;
	typedef P4  param4_type;
	typedef std::function<return_type(param1_type, param2_type, param3_type, param4_type)> functor;

	// constructors
	delegate() {}
	delegate(std::string key, const functor& f)
	{
		add(key, f);
	}

	delegate(const delegate& dlg)
	{
		*this += dlg;
	}

	delegate& operator+=(const delegate& dlg)
	{
		// The key MUST be unique.
		_f_containor.insert(dlg._f_containor.begin(), dlg._f_containor.end());
		return *this;
	}

	delegate& add(const std::string& key, const functor& f)
	{
		_f_containor[key] = f;
		return *this;
	}

	delegate& cancel(const std::string& key)
	{
		auto it = _f_containor.find(key);
		if (it != _f_containor.end())
		{
			_f_containor.erase(it);
		}
		return *this;
	}

	delegate& operator-=(const delegate& dlg)
	{
		if (this == &dlg)
		{
			_f_containor.clear();
		}
		else
		{
			for (auto it = dlg._f_containor.cbegin(); it != dlg._f_containor.cend(); it++)
			{
				cancel(it->first);
			}
		}
		return *this;
	}

	// Only return the first functor's return.
	return_type operator()(param1_type p1, param2_type p2, param3_type p3, param4_type p4)
	{
		if (_f_containor.empty())
		{
			return return_type();
		}

		for (auto it = ++_f_containor.cbegin(); it != _f_containor.cend(); it++)
		{
			it->second(p1, p2, p3, p4);
		}

		return _f_containor.cbegin()->second(p1, p2, p3, p4);
	}

private:
	std::map<std::string, functor> _f_containor;
};

CTS_END

#endif //_DELEGATE_H_
