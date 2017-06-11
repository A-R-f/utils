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

static Thread::return_type thread1_main(LPVOID arg)
{
	for(unsigned int i = 10 ; i > 0  ; --i )
	{
		ostringstream s;
		s << "thread1; i = " << i;
		display("thread1", s.str());
		Sleep(1000);
	}
	display("thread1", "thread1 done");
	return Thread::null_return();
}

static Thread::return_type thread2_main(LPVOID arg)
{
	for(unsigned int i = 10 ; i > 0  ; --i )
	{
		ostringstream s;
		s << "thread2; i = " << i;
		display("thread2", s.str());
		Sleep(702);
	}
	display("thread2", "thread2 done");
	return Thread::null_return();
}

int main()
{
	Thread thread1(thread1_main, NULL, true);
	Thread thread2(thread2_main, NULL, true);
	return 0;
}
