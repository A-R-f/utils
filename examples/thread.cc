/*
This is a simple demo how to use Thread and Mutex.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <sstream>
#include <linux/Mutex.hpp>
#include <linux/Thread.hpp>
#include <unistd.h>

using namespace std;

static void display(const string& owner, const string& s)
{
	static Mutex mutex;
	mutex.lock();
	cout << owner << " displays" << '\n';
	cout << '\t' << s << '\n';
	usleep(500000);
	cout << owner << " over and out" << '\n';
	mutex.unlock();
}

static void* thread1_main(void* const arg)
{
	for(unsigned int i = 10 ; i > 0  ; --i )
	{
		ostringstream s;
		s << "thread1; i = " << i;
		display("thread1", s.str());
		usleep(1000000);
	}
	display("thread1", "thread1 done");
	return NULL;
}

static void* thread2_main(void* const arg)
{
	for(unsigned int i = 10 ; i > 0  ; --i )
	{
		ostringstream s;
		s << "thread2; i = " << i;
		display("thread2", s.str());
		usleep(702303);
	}
	display("thread2", "thread2 done");
	return NULL;
}

int main()
{
	Thread thread1(thread1_main, NULL, true);
	Thread thread2(thread2_main, NULL, true);
	return 0;
}
