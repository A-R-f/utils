/*
Thread.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_THREAD_HPP
#define __LINUX_THREAD_HPP

#include <pthread.h>

class Thread {

	pthread_t thread;
	typedef void*(*thread_main_type)(void*);
	thread_main_type thread_main;
	void* arg;
	bool started;

public:

	Thread(const thread_main_type _thread_main, void* const _arg = NULL, const bool start = false) : thread_main(_thread_main), arg(_arg), started(false) { if( start) { run(); } }
	~Thread() { if( started ) { pthread_join(thread, NULL); } }

	int run() { return started = true, pthread_create(&thread, NULL, thread_main, arg); }
};

#endif
