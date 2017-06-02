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

	union FDes {
		int all[2];
		struct Named {
			int read;
			int write;
		} named;
	} fdes;

public:

	Pipe() { pipe(fdes.all); }
	~Pipe() { close(fdes.named.write); close(fdes.named.read); }

	int read(void* const data, const size_t cnt) const { return ::read(fdes.named.read, data, cnt); }
	int write(const void* const data, const size_t cnt) const { return ::write(fdes.named.write, data, cnt); }

	bool data_available() const { return InputPoll(fdes.named.read) > 0; }

	void flush() const { for( char c ; data_available() ; read(&c, 1) ); }

};

#endif
