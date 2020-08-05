/*
Serial.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017-2020
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __WINDOWS_SERIAL_HPP
#define __WINDOWS_SERIAL_HPP

#include <windows.h>


class Serial {

	std::string _port;
	HANDLE _fd;
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

	bool keep_reading_blocking(const char c) const { return ( c != '\0' ); }
	bool keep_reading_blocking(const char c, const char delim) const { return ( c != delim ) && keep_reading_blocking(c); }

	bool keep_reading(const char c) const { return keep_reading_blocking(c) && data_available(); }
	bool keep_reading(const char c, const char delim) const { return ( c != delim ) && keep_reading(c); }

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
		const WINBOOL rv = CloseHandle(_fd);
		_fd = INVALID_HANDLE_VALUE;
		return rv;
	}

	const HANDLE& fd() const { return _fd; }
	const std::string& port() const { return _port; }

	bool ok() const { return fd() != INVALID_HANDLE_VALUE; }

	bool data_available() const
	{
		COMSTAT _comstat;
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
		for( char c = '\0' + 1 ; keep_reading(c) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD read_blocking(std::string& s) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading_blocking(c) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD read(std::string& s, const int delim) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading(c, delim ) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD read_blocking(std::string& s, const int delim) const
	{
		s.clear();
		for( char c = '\0' + 1 ; keep_reading_blocking(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD read(std::string& s, const int delim, const size_t maxlen) const
	{
		s.clear();
		for( char c = '\0' + 1 ; ( s.length() < maxlen ) && keep_reading(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD read_blocking(std::string& s, const int delim, const size_t maxlen) const
	{
		s.clear();
		for( char c = '\0' + 1 ; ( s.length() < maxlen ) && keep_reading_blocking(c, delim) ; s += c ) { read((unsigned char*)&c, 1); }
		return s.length();
	}

	DWORD write(const unsigned char buf[], const unsigned int len) const
	{
		DWORD nw;
		WriteFile(_fd, buf, len, &nw, NULL);
		return nw;
	}

	DWORD write(const std::string& s) const { return write((unsigned char*)s.c_str(), s.length() + 1); }

	void flush_in() const { for( int c[1] ; data_available() ; read((unsigned char*)c, sizeof(c)) ); }
};

#endif
