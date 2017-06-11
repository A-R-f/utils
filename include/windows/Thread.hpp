/*
Thread.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __WINDOWS_THREAD_HPP
#define __WINDOWS_THREAD_HPP

#include <windows.h>

struct Thread {

	typedef DWORD WINAPI return_type;
	typedef return_type (*thread_main_type)(const LPVOID);
	static return_type null_return() { return 0; }

private:

	HANDLE thread;
	const thread_main_type thread_main;
	const LPVOID arg;
	bool started;

public:

	Thread(const thread_main_type _thread_main, const LPVOID _arg = NULL, const bool start = false) : thread_main(_thread_main), arg(_arg), started(false) { if( start) { run(); } }
	~Thread() { if( started ) { WaitForSingleObject(thread, INFINITE); CloseHandle(thread); } }

	HANDLE WINAPI run() { return started = true, thread = CreateThread(NULL, 0, thread_main, NULL, 0, NULL); }

};

#endif
