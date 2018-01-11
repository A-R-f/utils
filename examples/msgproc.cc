/*
This is a simple demo how to use Message_Processor.
Use as you wish, at your own risk.
*/

#include <Message_Processor.hpp>
#include <Tokenizer.hpp>
#include <iostream>


struct MSG_type {

	struct ID_type {
		int n;
		std::string s;
		ID_type(const int _n, const std::string& _s) : n(_n), s(_s) {}
		bool operator<(const ID_type& x) const { return ( n != x.n ) ? ( n < x.n ) : ( s < x.s ); }
		bool operator=(const ID_type& x) const { return ( n == x.n ) && ( s == x.s ); }
	};

	ID_type id;
	std::string contents;

	MSG_type(const int n = 0, const std::string& s1 = std::string(), const std::string& s2 = std::string()) : id(n, s1), contents(s2) {}
	operator const ID_type&() const { return id; }

};


struct MSG_Proc {

	bool operator()(MSG_type& msg)
	{
		std::cout << "last string was: " << msg.contents << std::endl;
		return false;
	}

};


Message_Processor<MSG_type::ID_type, MSG_type, MSG_Proc> msgproc;


int main()
{
	std::cout << "Message processor demo\n"
		  << "Type messages in the following format:\n"
		  << "\tnum string1 string2\n"
		  << "You get a one-time response for the following values:\n"
		  << "\tnum = 1, 3\n"
		  << "\tstring1 = ab, ba\n";

	msgproc.insert(MSG_type::ID_type(1, "ab"));
	msgproc.insert(MSG_type::ID_type(1, "ba"));
	msgproc.insert(MSG_type::ID_type(3, "ab"));
	msgproc.insert(MSG_type::ID_type(3, "ba"));

	while(1)
	{
		MSG_type msg;
		std::getline(std::cin, msg.contents);
		if( msg.contents == "quit" ) { return 0; }
		Tokenizer t;
		t << msg.contents;
		if( !t.get_token(msg.id.n) ) { continue; }
		if( !t.get_token(msg.id.s) ) { continue; }
		if( !t.get_token(msg.contents) ) { continue; }
		msgproc.process(msg);
	}
}
