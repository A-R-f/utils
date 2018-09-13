/*
This is a simple demo how to use Daemon.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <linux/Daemon.hpp>
#include <linux/Signal.hpp>

static volatile bool do_run = true;

static void kill_handler(int) { do_run = false; }

int main(int, char** argv)
{
	std::cout << "Now I'm a regular process, you can kill me with CTRL+C.\n";
	std::cout << "Press ENTER." << std::endl;
	std::cin.get();
	std::cout << "Now I'm going into background.\n";
	std::cout << "To kill me type:\n";
	std::cout << "\tkill `pidof " << argv[0] << '`' << std::endl;
	Daemon::verbose_detach(std::cerr);
	Signal sigint(SIGINT, kill_handler);
	while( do_run ) { usleep(100000); };
	return 0;
}
