/*
Signal.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_SIGNAL_HPP
#define __LINUX_SIGNAL_HPP

#include <signal.h>

class Signal {

	const int signo;
	struct sigaction sa;

public:

	Signal(const int _signo, sighandler_t _handler) :
		signo(_signo)
	{
		for( char* p = (char*)&sa + sizeof(sa) ; p != (char*)&sa ; ) { *--p = 0; }
		sa.sa_handler = _handler;
		sigaction(signo, &sa, &sa);
	}

	~Signal() { sigaction(signo, &sa, NULL); }

};

#endif
