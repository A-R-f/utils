/*
This is a simple demo how to use Tokenizer.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <Tokenizer.hpp>


template < typename T >
static void extract(Tokenizer& tok, const std::string typestr)
{
	std::cout << "extracting " << typestr << ": ";
	T t;
	if( tok.get_token(t) ) { std::cout << t; } else { std::cout <<  "failed"; }
	std::cout << '\n';
}


int main()
{
	Tokenizer tok;
	tok << "number 9 is here";
	extract<std::string>(tok, "string");
	extract<int>(tok, "int");
	extract<int>(tok, "int");
	return 0;
}
