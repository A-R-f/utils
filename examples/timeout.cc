/*
This is a simple demo how to use Timeout.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <linux/Timeout.hpp>


int main()
{
	std::cout << "Timeout in ms: ";
	unsigned int t;
	std::cin >> t;
	Timeout_ms tout(t);
	while( !tout );
	std::cout << "timeout!" << std::endl;
	return 0;
}
