/*
Daemon.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_DAEMON_HPP
#define __LINUX_DAEMON_HPP


#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <sys/stat.h>
#include <unistd.h>


struct Daemon {

	static int detach(const bool force_return = false, const bool return_errno = false)
	{
		const pid_t forkpid = fork();
		if( forkpid == 0 )	//child process to operate in background
		{
			setsid();
			const int fdnum = getdtablesize();
			for( int fd = 0 ; fd < fdnum ; ++fd ) { close(fd); }
			umask(027);
			chdir("/");
			return 0;
		}
		// forkpid > 0   - parent process
		// forkpid < 0   - error
		const int rv = ( forkpid > 0 )
			? (force_return ? forkpid : 0)
			: (return_errno ? -errno : forkpid);
		if( !force_return ) { exit(rv); }
		return rv;
	}

	static int verbose_detach(std::ostream& os, const bool force_return = false, const bool return_errno = false)
	{
		const int rv = detach(true, return_errno);
		if( rv < 0 ) { os << "detaching daemon failed: " << strerror(errno) << std::endl; }
		if( ( rv != 0 ) && !force_return ) { exit(( rv > 0 ) ? 0 : rv); }
		return rv;
	}
};


#endif
