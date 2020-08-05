/*
Serial.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017-2020
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_SERIAL_HPP
#define __LINUX_SERIAL_HPP

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <linux/Poll.hpp>
#include <string>


class Serial {

	int _fd;
	std::string _port;
	struct termios _opts;
//	struct termios _prevopts;

	static speed_t uint_to_speed(const unsigned int baud)
	{
		switch(baud)
		{
			case 50:	return B50;
			case 75:	return B75;
			case 110:	return B110;
			case 134:	return B134;
			case 150:	return B150;
			case 200:	return B200;
			case 300:	return B300;
			case 600:	return B600;
			case 1200:	return B1200;
			case 1800:	return B1800;
			case 2400:	return B2400;
			case 4800:	return B4800;
			case 9600:	return B9600;
			case 19200:	return B19200;
			case 38400:	return B38400;
			case 57600:	return B57600;
			case 115200:	return B115200;
			case 230400:	return B230400;
			case 460800:	return B460800;
			case 500000:	return B500000;
			case 576000:	return B576000;
			case 921600:	return B921600;
			case 1000000:	return B1000000;
			case 1152000:	return B1152000;
			case 1500000:	return B1500000;
			default:	return B0;
		}
	}

	void configure(const unsigned int baud)
	{
		fcntl(_fd, F_SETFL, 0);
		tcgetattr(_fd, &_opts);
//		_prevopts = _opts;
		const speed_t speed = uint_to_speed(baud);
		cfsetspeed(&_opts, speed);
		cfmakeraw(&_opts);
		_opts.c_cflag |= CREAD | CLOCAL;
		_opts.c_cflag &= ~CRTSCTS;
		if( tcsetattr(_fd, TCSANOW, &_opts) != 0 ) { close(); }
	}

//	void restore() const { tcsetattr(_fd, TCSANOW, &_prevopts); }

	bool keep_reading_blocking(const char c) const { return ( c != '\0' ); }
	bool keep_reading_blocking(const char c, const char delim) const { return ( c != delim ) && keep_reading_blocking(c); }

	bool keep_reading(const char c) const { return keep_reading_blocking(c) && data_available(); }
	bool keep_reading(const char c, const char delim) const { return ( c != delim ) && keep_reading(c); }

public:

	Serial(const std::string& dev, const unsigned int baud = 9600)
	{
		open(dev);
		if( ok() ) { configure(baud); }
	}

	~Serial()
	{
		if( ok() )
		{
//			restore();
			close();
		}
	}

	int open(const std::string& dev) { return _fd = ::open((_port = dev).c_str(), O_RDWR | O_NOCTTY); }
	int close() { return _fd = -2 - ::close(_fd); }

	int fd() const { return _fd; }
	const std::string& port() const { return _port; }

	bool ok() const { return fd() >= 0; }

	bool data_available() const { return InputPoll(_fd) > 0; }

	int read(unsigned char buf[], const unsigned int len) const { return ::read(_fd, buf, len); }

	int read(std::string& s) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading(c) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	int read_blocking(std::string& s) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading_blocking(c) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	int read(std::string& s, const int delim) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	int read_blocking(std::string& s, const int delim) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading_blocking(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	int read(std::string& s, const int delim, const size_t maxlen) const
	{
		s.clear();
		for( char c = '\0' + 1 ; ( s.length() < maxlen ) && keep_reading(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	int read_blocking(std::string& s, const int delim, const size_t maxlen) const
	{
		s.clear();
		for( char c = '\0' + 1 ; ( s.length() < maxlen ) && keep_reading_blocking(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

//template member function intended for use with std::vector<unsigned char>
	template < typename T >
	int read(T& buf, const unsigned char delim = 0, const bool keep_delim = true) const
	{
		int cnt = 0;
		unsigned char c;
		while(1)
		{
			read(&c, 1);
			if( ( c != delim ) || keep_delim )
			{
				buf.push_back(c);
				++cnt;
			}
			if( c == delim ) { break; }
		}
		return cnt;
	}

	int write(const unsigned char buf[], const unsigned int len) const { return ::write(_fd, buf, len); }
	int write(const std::string& s) const { return write((unsigned char*)s.c_str(), s.length() + 1); }
	int write(const char* const s) const { return write(std::string(s)); }
//template member function intended for use with std::vector<unsigned char>
	template < typename T > int write(const T& buf) const { return write(buf.data(), buf.size()); }

	void flush_in() const { tcflush(fd(), TCIFLUSH); }
	void wait_send() const { tcdrain(fd()); }
};


#endif
