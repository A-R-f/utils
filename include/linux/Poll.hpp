/*
Poll.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_POLL_HPP
#define __LINUX_POLL_HPP

#include <poll.h>

template<
	short EVENTS,
	nfds_t NUMBER = 1,
	int TIMEOUT = 0
>
class Poll {

	class Fds {

		pollfd _fds[NUMBER];

	public:

		Fds(const int fd[NUMBER])
		{
			for( nfds_t i = 0 ; i < NUMBER ; ++i )
			{
				_fds[i].fd = fd[i];
				_fds[i].events = EVENTS;
			}
		}

		operator pollfd*() { return _fds; }

	} fds;

public:

	Poll(const int& fd) : fds(&fd) {}
	Poll(const int fd[NUMBER]) : fds(fd) {}

	int operator()() { return execute(); }
	int execute() { return poll(fds, NUMBER, TIMEOUT); }
};

typedef Poll<POLLIN> InputPoll;
typedef Poll<POLLOUT> OutputPoll;

#endif