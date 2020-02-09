/*
This is a simple demo how to use UnixSocket.
Use as you wish, at your own risk.
*/

#include <linux/UnixSocket.hpp>
#include <iostream>


static void run_server()
{
	std::cout << "SERVER\n";
	std::cout << "path to inode: ";
	UnixSocket::Path path;
	std::cin >> path;
	UnixSocket::Server serv(path, true, true);
	std::cout << "server using inode " << serv.path() << std::endl;
	while(1)
	{
		if( serv.poll_in() && ( serv.accept() != NULL ) ) { std::cout << "got client" << std::endl; }
		UnixSocket::EP_Ptr_Set s = serv.poll_in_once();
		if( !s ) { usleep(1000); continue; }
		do
		{
			const UnixSocket::Endpoint* ep = s.pop_first();
			const std::string msg = ep->recv_str();
			if( msg.empty() ) { continue; }
			std::cout << "message (" << msg.length() << " bytes): " << msg << std::endl;
			if( msg == "quit" ) { return; }
		}
		while( s );
	}
}


static void run_client()
{
	std::cout << "CLIENT\n";
	std::cout << "connect to inode: ";
	UnixSocket::Path path;
	std::cin >> path;
	UnixSocket::Client cl(path, true, true);
	std::cout << "client connected to " << cl.path() << std::endl;
	usleep(10000);
	ssize_t l = cl.send("Good morning, Server!");
	std::cout << "sent " << l << " characters" << std::endl;
	usleep(1000000);
	l = cl.send("How are you today?");
	std::cout << "sent " << l << " characters" << std::endl;
	usleep(1000000);
	l = cl.send("quit");
	std::cout << "sent " << l << " characters" << std::endl;
}


int main()
{
	for( ; ; )
	{
		std::cout << "Is this a server or client? (s/c)" << std::endl;
		char role;
		std::cin >> role;
		if( role == 's' )
		{
			run_server();
			break;
		}
		if( role == 'c' )
		{
			run_client();
			break;
		}
	}
	
	return 0;
}
