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


class Daemon {

	static int config_child(const bool close_all_fd = false)
	{
		setsid();
		if( close_all_fd )		//close all file descriptors
		{
			const int fdnum = getdtablesize();
			for( int fd = 0 ; fd < fdnum ; ++fd ) { close(fd); }
		}
		else				//or only stdio
		{
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
		}
		umask(027);
		chdir("/");
		return 0;
	}

	static int parent_or_error_rv(const pid_t forkpid, const bool force_return, const bool return_errno)
	{
		const int rv = ( forkpid > 0 )
			? (force_return ? forkpid : 0)		// forkpid > 0   - parent process
			: (return_errno ? -errno : forkpid);	// forkpid < 0   - error
		if( !force_return ) { exit(rv); }
		return rv;
	}

public:

	static int detach(const bool force_return = false, const bool return_errno = false)
	{
		const pid_t forkpid = fork();

		if( forkpid == 0 ) { return config_child(); }	//child process to operate in background

		// forkpid > 0   - parent process
		// forkpid < 0   - error
		return parent_or_error_rv(forkpid, force_return, return_errno);
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
