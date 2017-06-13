/*
Pipe.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_PIPE_HPP
#define __LINUX_PIPE_HPP

#include <linux/Poll.hpp>
#include <unistd.h>

class Pipe {

	struct FDesc {
		int fd[2];
		int read() const { return fd[0]; }
		int write() const { return fd[1]; }
		operator int*() { return fd; }
	} fdesc;

public:

	Pipe() { pipe(fdesc); }
	~Pipe() { close(fdesc.write()); close(fdesc.read()); }

	int read(void* const data, const size_t cnt) const { return ::read(fdesc.read(), data, cnt); }
	int write(const void* const data, const size_t cnt) const { return ::write(fdesc.write(), data, cnt); }

	bool data_available() const { return InputPoll(fdesc.read()) > 0; }

	void flush() const { for( char c ; data_available() ; read(&c, 1) ); }

};

#endif
