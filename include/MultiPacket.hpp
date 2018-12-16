/*
MultiPacket.hpp
General frame intended for handling packets with a number of recipients
or getting data from a number of sources
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __MULTIPACKET_HPP
#define __MULTIPACKET_HPP


#include <functional>
#include <vector>


template <
	class Packet,
	class Control,
	class Timeout
>
struct MultiPacket {

	typedef Packet packet_type;
	typedef std::vector<unsigned char> payload_type;
	typedef Control control_type;

	typedef std::function<void(payload_type&, control_type&, const control_type&)> Constructor;
	typedef std::function<bool(payload_type&, control_type&, const packet_type&)> Matcher;
	typedef std::function<void(payload_type&)> Sender;

private:

	payload_type payload;
	control_type control;

	Constructor __init;
	Matcher __match;
	Sender __send;
	Timeout __timeout;

public:

	MultiPacket(Constructor _c, Matcher _m, Sender _s):
		__init(_c),
		__match(_m),
		__send(_s),
		__timeout(0)
	{}

	void init(const control_type& _ctrl, const Timeout& _tout = Timeout(1))
	{
		__init(payload, control, _ctrl);
		__timeout = _tout;
	}

	bool match(const packet_type& _pkt) { return __match(payload, control, _pkt); }

	bool ready() const { return control.ready(); }

	void send() { __send(payload); }

	bool timeout() const { return __timeout(); }

};


#endif
