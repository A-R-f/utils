/*
Signal.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017-2019
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_SIGNAL_HPP
#define __LINUX_SIGNAL_HPP

#include <cstring>
#include <signal.h>

class Signal {

	const int signo;
	struct sigaction sa;

public:

	Signal(const int _signo, sighandler_t _handler) :
		signo(_signo)
	{
		struct sigaction new_sa;
		bzero(&new_sa, sizeof(new_sa));
		new_sa.sa_handler = _handler;
		sigaction(signo, &new_sa, &sa);
	}

	~Signal() { sigaction(signo, &sa, NULL); }

};

#endif
