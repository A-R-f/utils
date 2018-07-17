#ifndef __LINUX_PIDFILE_HPP
#define __LINUX_PIDFILE_HPP


#include <unistd.h>
#include <RunTimeFile.hpp>

struct PIDFile : public RunTimeFile {

	PIDFile(const std::string& path) : RunTimeFile(path, getpid()) {}

};


#endif
