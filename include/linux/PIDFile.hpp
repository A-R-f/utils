/*
PIDFile.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_PIDFILE_HPP
#define __LINUX_PIDFILE_HPP


#include <unistd.h>
#include <RunTimeFile.hpp>

struct PIDFile : public RunTimeFile {

	PIDFile(const std::string& path) : RunTimeFile(path, getpid()) {}

};


#endif
