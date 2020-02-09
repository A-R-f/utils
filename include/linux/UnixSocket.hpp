/*
UnixSocket.hpp
Author: Adam Rudziński, devel@arf.net.pl
Copyright: Adam Rudziński, 2020
This is free software, licensed under GNU GPLv3 license.
This software comes with ABSOLUTELY NO WARRANTY, USE AT YOUR OWN RISK!
*/

#ifndef __LINUX_UNIXSOCKET_HPP
#define __LINUX_UNIXSOCKET_HPP

#include <cstring>
#include <linux/Poll.hpp>
#include <set>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


struct UnixSocket {

	typedef std::string Path;

	struct Endpoint {

	protected:

		int fd;

		int open(const bool nonblock = false) { return fd = socket(AF_UNIX, SOCK_SEQPACKET | (nonblock ? SOCK_NONBLOCK : 0), 0); }

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

		ssize_t send(const std::string& msg) const
		{
			ssize_t len = 0;
			size_t tosend = msg.length();
			const char* const ptr = msg.c_str();
			while( tosend > 0 )
			{
				const size_t seg = ( tosend > 16 ) ? 16 : tosend;
				const ssize_t n = write(ptr + len, seg);
				if( n < 0 ) { return n; }
				len += n;
				tosend -= n;
			}
			return len;
		}

		int poll_in() const { return Poll<POLLIN, 1, 0>(fd); }

		ssize_t read(void* const buf, const size_t len) const { return ::read(fd, buf, len); }

		ssize_t recv(std::string& msg) const
		{
			msg.clear();
			ssize_t len = 0;
			for( ;  ; )
			{
				if( !poll_in() ) { break; }
				char buf[16];
				ssize_t n = read(buf, sizeof(buf));
				if( n <= 0 ) { break; }
				for( ssize_t i = 0 ; i < n ; ++i ) { msg += buf[i]; }
				len += n;
			}
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

		struct sockaddr_un serv_addr;

		Base(const bool autoopen = false, const bool nonblock = false)
		{
			memset(&serv_addr, 0, sizeof(serv_addr));
			serv_addr.sun_family = AF_UNIX;
			if( !autoopen ) { return; }
			open(nonblock);
		}

	public:

		~Base() { close(); }

		int open(const bool nonblock = false) { return Endpoint::open(nonblock); }
		int close()
		{
			const int rv = Endpoint::close();
			unlink(serv_addr.sun_path);
			return rv;
		}

		Path path() const { return serv_addr.sun_path; }

		void path(const Path& p) { strncpy(serv_addr.sun_path, p.c_str(), sizeof(serv_addr.sun_path) - 1); }

	};

public:


	class Server : public Base {

		struct Connections : private EP_Ptr_Set {

			~Connections()
			{
				for( EP_Ptr_Set::iterator i = EP_Ptr_Set::begin() ; i != EP_Ptr_Set::end() ; ++i ) { delete *i; }
			}

			operator const EP_Ptr_Set&() { return *(const EP_Ptr_Set*)(this); }

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

		Server(const Path& p = Path(), const bool autostart = false, const bool nonblock = false) : Base(autostart, nonblock)
		{
			path(p);
			if( !autostart ) { return; }
			bind();
			listen();
		}

		const Endpoint* accept() { return conns.insert(::accept(fd, NULL, NULL)); }

		int bind() const { return ::bind(fd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)); }

		int listen(const int backlog = 5) const { return ::listen(fd, backlog); }

		EP_Ptr_Set poll_in_once() const { return conns.poll_in_once(); }

		const EP_Ptr_Set& connected() const { return (const EP_Ptr_Set&)conns; }

	};


	struct Client : public Base {

		Client(const Path& p, const bool autostart = false, const bool nonblock = false) : Base(autostart, nonblock)
		{
			path(p);
			if( !autostart ) { return; }
			connect();
		}

		bool connect() const { return ::connect(fd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0; }

	};


};


#endif
