/*
This is a simple demo how to use Serial.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <sstream>
#include <linux/Serial.hpp>

using namespace std;

int main()
{
	Serial sp("/dev/ttyUSB0", 115200);
	cout << "FD: " << sp.fd() << endl;
	sp.write("v");
	usleep(100000);
	unsigned char buf[21];
	int n = sp.read(buf, 20);
	buf[n] = '\0';
	cout << "READ: " << buf << endl;
	return 0;
}
