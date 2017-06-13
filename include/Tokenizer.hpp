/*
Tokenizer.hpp
Class for extracting tokens of given types from a stream
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2017
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __TOKENIZER_HPP
#define __TOKENIZER_HPP

#include <sstream>

struct Tokenizer : private std::stringstream {

//methods for checking state
	bool fail() const { return std::stringstream::fail(); }
	std::string str() const { return std::stringstream::str(); }

//formatted input
	template < typename T >
	Tokenizer& operator<<(const T& t)
	{
		(std::stringstream&)(*this) << t;
		return *this;
	}

//get a token via formatted output without error checking
	template < typename T >
	Tokenizer& operator>>(T& t)
	{
		(std::stringstream&)(*this) >> t;
		return *this;
	}

//get a token via formatted output with error checking
//returns true if the token was successfully retrieved
	template < typename T >
	bool get_token(T& t)
	{
		*this >> t;
		return !fail();
	}

};

#endif
