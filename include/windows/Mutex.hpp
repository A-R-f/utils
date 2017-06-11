/*
Mutex.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __WINDOWS_MUTEX_HPP
#define __WINDOWS_MUTEX_HPP

#include <windows.h>

class Mutex {

	HANDLE mutex;

public:

	Mutex() { mutex = CreateMutex(NULL, FALSE, NULL); }
	~Mutex() { CloseHandle(mutex); }

	bool lock() const { return WaitForSingleObject(mutex, INFINITE) == WAIT_OBJECT_0; }
	bool unlock() const { return ReleaseMutex(mutex); }

};

#endif
