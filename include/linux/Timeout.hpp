/*
Timeout.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_TIMEOUT_HPP
#define __LINUX_TIMEOUT_HPP

#include <ctime>

template< unsigned int UNITS_PER_SECOND, clockid_t CLOCK = CLOCK_MONOTONIC_RAW >
struct Timeout {

	struct Time : private timespec {

		Time() { clock_gettime(CLOCK, this); }

		Time(const unsigned int t)
		{
			tv_sec = t / UNITS_PER_SECOND;
			tv_nsec = (t - tv_sec * UNITS_PER_SECOND) * (1e9 / UNITS_PER_SECOND);
		}

		operator unsigned int() const
		{
			const unsigned int DIV = 1.0e9 / UNITS_PER_SECOND;
			return UNITS_PER_SECOND * tv_sec + (tv_nsec + DIV/2) / DIV;
		}

		Time& operator+=(const Time& t)
		{
			tv_sec += t.tv_sec;
			tv_nsec += t.tv_nsec;
			if( tv_nsec > 1e9 )
			{
				tv_nsec -= 1e9;
				tv_sec += 1;
			}
			return *this;
		}

		Time operator+(const Time& t) const { return Time(*this) += t; }

		bool operator>(const Time& t) const { return ( tv_sec == t.tv_sec ) ? ( tv_nsec > t.tv_nsec ) : ( tv_sec > t.tv_sec ); }

	};

private:

	Time timeout_time;

public:

	Timeout(const unsigned int val) : timeout_time(Time() + Time(val)) {}

	Timeout& operator+=(const unsigned int val) { timeout_time += Time(val); return *this; }

	operator bool() const { return Time() > timeout_time; }
	bool timeout() const { return (*this)(); }
	bool timeout(const Time& t) const { return t > timeout_time; }

};


typedef Timeout<1> Timeout_s;
typedef Timeout<1000> Timeout_ms;
typedef Timeout<1000000> Timeout_us;

#endif
