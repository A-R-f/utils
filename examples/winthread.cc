/*
This is a simple demo how to use Thread and Mutex.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <sstream>
#include <windows/Mutex.hpp>
#include <windows/Thread.hpp>
#include <unistd.h>

using namespace std;

static void display(const string& owner, const string& s)
{
	static Mutex mutex;
	mutex.lock();
	cout << owner << " displays" << '\n';
	cout << '\t' << s << '\n';
	Sleep(500);
	cout << owner << " over and out" << '\n';
	mutex.unlock();
}

struct Thread_Config {
	string name;
	unsigned int sleep_time;
	unsigned int cnt_start;
	Thread_Config(const string& _n, const unsigned int _st, const unsigned int _cs) :
		name(_n),
		sleep_time(_st),
		cnt_start(_cs)
	{}
};

static Thread::return_type thread_main(const Thread::arg_type arg)
{
	Thread_Config& cfg = *(Thread_Config*)arg;
	for(unsigned int i = cfg.cnt_start ; i > 0  ; --i )
	{
		ostringstream s;
		s << cfg.name << "; i = " << i;
		display(cfg.name, s.str());
		Sleep(cfg.sleep_time);
	}
	display(cfg.name, "I'm done");
	return Thread::null_return();
}

int main()
{
	Thread_Config tc1("thread1", 1000, 11);
	Thread_Config tc2("thread2", 720, 9);
	Thread thread1(thread_main, &tc1, true);
	Thread thread2(thread_main, &tc2, true);
	return 0;
}
