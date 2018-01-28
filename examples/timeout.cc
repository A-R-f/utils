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
	tout += t;
	for( Timeout_ms::Time tout_time ; !tout.timeout(tout_time) ; tout_time = Timeout_ms::Time() );
	std::cout << "and again!" << std::endl;
	return 0;
}
