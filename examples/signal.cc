/*
This is a simple demo how to use Signal.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <linux/Signal.hpp>
#include <unistd.h>

static volatile bool do_run;

static void sig_handler(int) { do_run = false; }

static void loop1()
{
	do_run = true;
	Signal sigint(SIGINT, sig_handler);
	std::cout << "\n1. Now CTRL+C will NOT terminate the process.\n";
	std::cout << "Press CTRL+C." << std::endl;
	while( do_run );
}

static void loop2()
{
	std::cout << "\n2. Now CTRL+C will terminate the process.\n";
	std::cout << "Press CTRL+C until countdown is finished or wait." << std::endl;
	std::cout << "countdown:" << std::flush;
	unsigned int i = 10;
	do
	{
		usleep(500000);
		std::cout << ' ' << i << std::flush;
	}
	while( i-- );
	std::cout << '\n';
	std::cout << "You didn't press CTRL+C.\n";
}

int main()
{
	std::cout << "\nSignal demo start\n";
	loop1();
	loop2();
	std::cout << "\nSignal demo end\n";
	return 0;
}
