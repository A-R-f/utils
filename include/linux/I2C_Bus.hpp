/*
I2C_Bus.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2019
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __I2C_BUS_HPP
#define __I2C_BUS_HPP

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>


class I2C_Bus {

	int _fd;
	std::string _dev;

public:

	I2C_Bus(const std::string& _d)
	{
		open(_d);
	}

	~I2C_Bus()
	{
		if( ok() ) { close(); }
	}

	int open(const std::string& _d) { return _fd = ::open((_dev = _d).c_str(), O_RDWR); }
	int close() { return _fd = -2 - ::close(_fd); }

	int fd() const { return _fd; }
	const std::string& dev() const { return _dev; }

	bool ok() const { return fd() >= 0; }

	bool select_slave(const unsigned int addr) { return ioctl(fd(), I2C_SLAVE, addr) >= 0; }

	int read(unsigned char buf[], const unsigned int len) const { return ::read(_fd, buf, len); }

	int write(const unsigned char buf[], const unsigned int len) const { return ::write(_fd, buf, len); }
};


#endif
