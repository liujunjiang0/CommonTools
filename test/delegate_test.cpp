#include <iostream>
#include <memory>

#include <delegate.h>

using namespace cts;

void callback00()
{
	std::cout << "None params callback 00" << std::endl;
}

void callback01()
{
	std::cout << "None params callback 01" << std::endl;
}

class mem_func
{
public:
	void subscribe(const std::string& key, const std::function<int(int)>& f)
	{
		_dlg.add(key, f);
	}
	void unsubscribe(const std::string& key)
	{
		_dlg.cancel(key);
	}

	void run(int a)
	{
		_dlg(a);
	}
private:
	delegate<int(int)> _dlg;
};

class func1_class 
{
public:
	func1_class(mem_func& m) : _mem(m)
	{
	}
	~func1_class()
	{
		_mem.unsubscribe("func1_class::callback2");
	}

	void setup() 
	{
		_mem.subscribe("func1_class::callback1", std::bind(&func1_class::callback1, this, std::placeholders::_1));
		_mem.subscribe("func1_class::callback2", std::bind(&func1_class::callback2, this, std::placeholders::_1));
		//Duplicate add will overwrite the former one.
		_mem.subscribe("func1_class::callback2", std::bind(&func1_class::callback2, this, std::placeholders::_1));
	}
	void cancel()
	{
		_mem.unsubscribe("func1_class::callback1");
	}
	
private:
	int callback1(int a) { std::cout << "callback1, a = " << a << std::endl; return a; }
	int callback2(int b) { std::cout << "callback2, b = " << b << std::endl; return b; }
	mem_func& _mem;
};


int main()
{
	delegate<void()> d00;
	delegate<void()> d01;
	d00.add("callback00", std::bind(callback00));
	d00.add("callback01", std::bind(callback01));
	d01.add("callback01", std::bind(callback01));
	d00();         //01 00, two callbacks
	d01();	       //01
	d00 -= d01;    //00
	d00();
	d00 += d01;    //01 00   
	d00 += d01;    //Cannot add duplicate keys.
	d00();         //01 00
/* Print:
None params callback 01
None params callback 00
None params callback 01
None params callback 00
None params callback 01
None params callback 00
*/

	mem_func m;
	func1_class cls(m);
	cls.setup();
	m.run(7);        //callback1, callback2
	cls.cancel();
	m.run(7);        //callback2
/*
callback2, b = 7
callback1, a = 7
callback2, b = 7
*/	
	return 0;
}
