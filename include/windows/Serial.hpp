/*
Serial.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __WINDOWS_SERIAL_HPP
#define __WINDOWS_SERIAL_HPP

#include <windows.h>


class Serial {

	std::string _port;
	HANDLE _fd;
	COMSTAT _comstat;
	DCB _dcb;

	void configure(const unsigned int baud)
	{
		SecureZeroMemory(&_dcb, sizeof(_dcb));
		_dcb.DCBlength = sizeof(_dcb);
		GetCommState(_fd, &_dcb);
		_dcb.BaudRate = baud;
		_dcb.ByteSize = 8;
		_dcb.Parity = NOPARITY;
		_dcb.StopBits = ONESTOPBIT;
		SetCommState(_fd, &_dcb);
	}

//	void restore() const { SetCommState(_fd, &old_dcb); }

public:

	Serial(const char* const dev, const unsigned int baud = 9600) :
		_port(dev)
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

	HANDLE open(const char* const dev) { return _fd = CreateFile(dev, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); }
	WINBOOL close()
	{
		_fd = INVALID_HANDLE_VALUE;
		return CloseHandle(_fd);
	}

	const HANDLE& fd() const { return _fd; }
	const std::string& port() const { return _port; }

	bool ok() const { return fd() != INVALID_HANDLE_VALUE; }

	bool data_available()
	{
		ClearCommError(_fd, 0, &_comstat);
		return _comstat.cbInQue > 0;
	}

	DWORD read(unsigned char buf[], const unsigned int len) const
	{
		DWORD nr;
		ReadFile(_fd, buf, len, &nr, NULL);
		return nr;
	}

	DWORD read(std::string& s) const
	{
		s.clear();
		for( char c = ~('\0' | '\n') ; c != '\n' ; s += c )
		{
			read((unsigned char*)&c, 1);
			if( c == '\0' ) { break; }
		}
		return s.length();
	}

	DWORD write(const unsigned char buf[], const unsigned int len) const
	{
		DWORD nw;
		WriteFile(_fd, buf, len, &nw, NULL);
		return nw;
	}

	DWORD write(const std::string& s) const { return write((unsigned char*)s.c_str(), s.length()); }
};

#endif
