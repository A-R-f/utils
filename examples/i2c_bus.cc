/*
This is a simple demo how to use I2C_Bus.
Use as you wish, at your own risk.
*/

#include <cstdlib>
#include <iostream>
#include <linux/I2C_Bus.hpp>

using namespace std;


int main(int argc, char** argv)
{
	if( argc < 2 )
	{
		cout << "Usage:\n"
		     << '\t' << argv[0] << " <bus_number>" << endl;
		exit(0);
	}

	std::string devpath("/dev/i2c-");
	devpath += argv[1];

	I2C_Bus bus(devpath);

	if( !bus.ok() )
	{
		cout << "Error opening I2C device " << devpath << endl;
		exit(1);
	}

	bus.select_slave(0x18);

	const unsigned char obuf[] = "Hello I2C";
	unsigned char ibuf[10];

	bus.write(obuf, sizeof(obuf));
	bus.read(ibuf, sizeof(ibuf));

	cout << "read:";
	for( size_t i = 0 ; i < sizeof(ibuf); ++i ) { cout << " 0x" << hex << (unsigned int)(ibuf[i]) << dec; }
	cout << endl;

	return 0;
}
