/*
Message_Processor.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __MESSAGE_PROCESSOR_HPP
#define __MESSAGE_PROCESSOR_HPP


#include <set>


template <
	typename ID_t,
	typename MSG_t,
	class Processor,
	class Order = std::less<ID_t>
>
struct Message_Processor : public std::set<ID_t, Order> {

	typedef std::set<ID_t, Order> Set_Type;

	bool process(MSG_t& msg, const bool autoerase = true)
	{
		const typename Set_Type::iterator i = Set_Type::find(msg);
		if( i == Set_Type::end() ) { return false; }
		const bool keep = Processor()(msg);
		if( !keep && autoerase ) { Set_Type::erase(i); }
		return true;
	}

};


#endif
