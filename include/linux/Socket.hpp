/*
Socket.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2018
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_SOCKET_HPP
#define __LINUX_SOCKET_HPP

#include <cstdio>
#include <cstring>
#include <linux/Poll.hpp>
#include <linux/tcp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <set>
#include <stdint.h>
#include <string>
#include <sys/socket.h>
//#include <sys/types.h>
#include <unistd.h>
#include <iostream>


struct Socket {

	typedef u_short Port;

	struct Endpoint {

	protected:

		int fd;

		int open(const bool nonblock = false) { return fd = socket(AF_INET, SOCK_STREAM | (nonblock ? SOCK_NONBLOCK : 0), 0); }

		int close()
		{
			const int rv = ::close(fd);
			fd = -1;
			return rv;
		}

	public:

		Endpoint(const int desc = -1) : fd(desc) {}

		~Endpoint() { close(); }

		bool operator<(const Endpoint& ep) const { return fd < ep.fd; }
		bool operator==(const int _fd ) const { return fd == _fd; }

		int poll_out() const { return Poll<POLLOUT, 1, 0>(fd); }

		ssize_t write(const void* const buf, const size_t len) const { return ::write(fd, buf, len); }

		ssize_t send(const std::string msg) const { return write(msg.c_str(), msg.length()); }

		void set_delay(const bool d = false) const
		{
			const int flag = d ? 0 : 1;
			setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
		}

		int poll_in() const { return Poll<POLLIN, 1, 0>(fd); }

		ssize_t read(void* const buf, const size_t len) const { return ::read(fd, buf, len); }

		ssize_t recv(std::string& msg) const
		{
			msg.clear();
			ssize_t len = 0;
			for( char c ; poll_in() && read(&c, 1) ; msg += c, ++len );
			return len;
		}

		std::string recv_str() const
		{
			std::string msg;
			recv(msg);
			return msg;
		}

	};

	struct EP_Ptr_less {
		bool operator()(const Endpoint* const ep1, const Endpoint* const ep2)
		{
			return ( ep1 == NULL ) ? true :
			       ( ep2 == NULL ) ? false :
			       *ep1 < *ep2;
		}
	};

	struct EP_Ptr_Set : public std::set<const Endpoint*, EP_Ptr_less>
	{
		typedef std::set<const Endpoint*, EP_Ptr_less> Base_Set;

		operator bool() const { return !empty(); }

		const Endpoint* pop_first()
		{
			if( empty() ) { return NULL; }
			const Endpoint* const rv = *begin();
			erase(begin());
			return rv;
		}

		std::pair<iterator, bool> insert(const value_type& v) { return Base_Set::insert(v); }

		void insert(const EP_Ptr_Set& s) { Base_Set::insert(s.begin(), s.end()); }
	};

private:

	struct Base : public Endpoint {

	protected:

		struct sockaddr_in serv_addr;

		Base(const bool autoopen = false, const bool nonblock = false)
		{
			memset(&serv_addr, 0, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = INADDR_ANY;
			if( !autoopen ) { return; }
			open(nonblock);
		}

	public:

		int open(const bool nonblock = false) { return Endpoint::open(nonblock); }
		int close() { return Endpoint::close(); }

		Port port() const { return ntohs(serv_addr.sin_port); }

		void port(const Port p) { serv_addr.sin_port = htons(p); }

	};

public:


	class Server : public Base {

		struct Connections : private EP_Ptr_Set {

			~Connections()
			{
				for( EP_Ptr_Set::iterator i = EP_Ptr_Set::begin() ; i != EP_Ptr_Set::end() ; ++i ) { delete *i; }
			}

			const Endpoint* insert(const int fd)
			{
				if( fd < 0 ) { return NULL; }
				const Endpoint* const ep = new Endpoint(fd);
				EP_Ptr_Set::insert(ep);
				return ep;
			}

			void erase(const int fd)
			{
				for( EP_Ptr_Set::iterator i = EP_Ptr_Set::begin() ; i != EP_Ptr_Set::end() ; ++i )
				{
					const Endpoint* ep = *i;
					if( *ep == fd )
					{
						EP_Ptr_Set::erase(i);
						delete ep;
						return;
					}
				}
			}

			EP_Ptr_Set poll_in_once() const
			{
				EP_Ptr_Set s;
				for( EP_Ptr_Set::const_iterator i = EP_Ptr_Set::begin() ; i != EP_Ptr_Set::end() ; ++i )
				{
					const Endpoint* const ep = *i;
					if( ep->poll_in() ) { s.insert(ep); }
				}
				return s;
			}

		};

	private:

		Connections conns;

	public:

		Server(const Port p = 0, const bool autostart = false, const bool nonblock = false) : Base(autostart, nonblock)
		{
			port(p);
			if( !autostart ) { return; }
			bind();
			listen();
		}

		const Endpoint* accept() { return conns.insert(::accept(fd, NULL, NULL)); }

		int bind() const { return ::bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); }

		int listen(const int backlog = 5) const { return ::listen(fd, backlog); }

		EP_Ptr_Set poll_in_once() const { return conns.poll_in_once(); }

	};


	struct Client : public Base {

		Client(const std::string hostname, const Port p, const bool autostart = false, const bool nonblock = false) : Base(autostart, nonblock)
		{
			gethost(hostname);
			port(p);
			if( !autostart ) { return; }
			connect();
		}

		int connect() const { return ::connect(fd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)); }

		bool gethost(const std::string& name)
		{
			memset(&serv_addr.sin_addr.s_addr, 0, sizeof(serv_addr.sin_addr.s_addr));
			const struct hostent* const host = gethostbyname(name.c_str());
			if( host == NULL ) { return false; }
			memmove(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length);
			return true;;
		}

		unsigned int hostaddr() const { return serv_addr.sin_addr.s_addr; }

		std::string hostaddr_str() const
		{
			char buf[4*3 + 3*1 + 1];
			std::sprintf(buf, "%d.%d.%d.%d", (hostaddr() >> 0) & 0xff, (hostaddr() >> 8) & 0xff, (hostaddr() >> 16) & 0xff, (hostaddr() >> 24) & 0xff);
			return buf;
		}

	};


};


#endif
