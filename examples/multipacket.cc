#include <MultiPacket.hpp>
#include <functional>
#include <iostream>
#include <vector>
#include <linux/Timeout.hpp>


struct Control {
	std::vector<unsigned int> pending_ranks;
	bool ready() const { return pending_ranks.empty(); }
};

struct Packet {
	unsigned int length;
	unsigned int rank;
	std::string name;
	struct Raw {
		unsigned int length;
		unsigned int rank;
		char name[1];
		const Raw* next() const { return (const Raw*)((unsigned char*)this + length); }
	};
};

typedef MultiPacket < Packet, Control, Timeout_s > MPack;

void init(MPack::payload_type& pd, MPack::control_type& ctrl, const MPack::control_type& _c)
{
	ctrl = _c;
	pd.clear();
}

bool match(MPack::payload_type& pd, MPack::control_type& ctrl, const Packet& _pkt)
{
	for( std::vector<unsigned int>::iterator i = ctrl.pending_ranks.begin() ; i != ctrl.pending_ranks.end() ; ++i )
	{
		if( *i == _pkt.rank )
		{
			ctrl.pending_ranks.erase(i);
			const size_t m = pd.size();
			pd.resize(m + _pkt.length);
			Packet::Raw* const p = (Packet::Raw*)(pd.data() + m);
			p->length = _pkt.length;
			p->rank = _pkt.rank;
			const size_t k = _pkt.name.length();
			for( size_t j = 0 ; j < k ; ++j ) { p->name[j] = _pkt.name.at(j); }
			p->name[k] = '\0';
			return true;
		}
	}
	return false;
}

void send(MPack::payload_type& pd)
{
	ssize_t n = pd.size();
	for( const Packet::Raw* p = (const Packet::Raw*)pd.data() ; (unsigned char*)p - pd.data() < n ; p = p->next() )
	{
		std::cout << p->rank << ". " << p->name << '\n';
	}
}

int main()
{
	MPack mp(init, match, send);

	std::cout << "Enter sequence of ranks (numbers), end with -1: ";
	MPack::control_type c;
	while(1)
	{
		int r;
		std::cin >> r;
		if( r < 0 ) { break; }
		c.pending_ranks.push_back(r);
	}

	mp.init(c);

	Packet p;
	while( !mp.ready() )
	{
		std::cout << "rank: ";
		std::cin >> p.rank;
		std::cout << "name: ";
		std::cin >> p.name;
		p.length = sizeof(p.length) + sizeof(p.rank) + p.name.length() + 1;
		if( mp.match(p) ) { std::cout << "this was needed\n"; }
		else { std::cout << "this is ignored\n"; }
	}
	mp.send();

	return 0;
}
