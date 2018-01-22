/*
Message_Relay.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __MESSAGE_RELAY_HPP
#define __MESSAGE_RELAY_HPP


#include <map>


template <
	typename ID_t,
	typename Recipients,
	typename MSG_t,
	class Relay,
	class Order = std::less<ID_t>
>
struct Message_Relay : public std::map<ID_t, Recipients, Order> {

	typedef std::map<ID_t, Recipients, Order> Map_Type;

	Recipients* check_recipients(const ID_t& id)
	{
		const typename Map_Type::iterator i = Map_Type::find(id);
		if( i == Map_Type::end() ) { return NULL; }
		return &(i->second);
	}

	Recipients& recipients(const ID_t& id)
	{
		const typename Map_Type::iterator i = Map_Type::find(id);
		return ( i == Map_Type::end() ) ? Map_Type::insert(typename Map_Type::value_type(id, Recipients())).first->second : i->second;
	}

	bool relay(MSG_t& msg, const bool autoerase = true)
	{
		const typename Map_Type::iterator i = Map_Type::find(msg);
		if( i == Map_Type::end() ) { return false; }
		const bool keep = Relay()(msg, i->second);
		if( !keep && autoerase ) { Map_Type::erase(i); }
		return true;
	}

};


#endif
