/*
Mutex.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_MUTEX_HPP
#define __LINUX_MUTEX_HPP

#include <pthread.h>

class Mutex {

	pthread_mutex_t mutex;

public:

	Mutex() { pthread_mutex_init(&mutex, NULL); }
	~Mutex() { pthread_mutex_destroy(&mutex); }

	int lock() { return pthread_mutex_lock(&mutex); }
	int unlock() { return pthread_mutex_unlock(&mutex); }

};

#endif
