/*
This is a simple demo how to use Serial for Windows.
Use as you wish, at your own risk.
*/

#include <iostream>
#include <sstream>
#include <windows/Serial.hpp>

using namespace std;

int main(int argc, char** argv)
{
	Serial sp(argv[1], 115200);
	cout << "FD: " << sp.fd() << endl;
	sp.write("v");
	Sleep(100);
	unsigned char buf[21];
	int n = sp.read(buf, 20);
	buf[n] = '\0';
	cout << "READ: " << buf << endl;
	return 0;
}
