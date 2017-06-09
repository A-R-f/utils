/*
This is a simple demo how to use COBS.
Use as you wish, at your own risk.
*/

#include <COBS.hpp>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<unsigned char> buf_type;

int main()
{
	buf_type buf;

	cout << "Enter data: \n";
	while( 1 )
	{
		int c;
		cin >> c;
		if( c < 0 ) { break; }
		buf.push_back(c);
	}

	cout << "raw data: \n" << hex;
	for( unsigned int i = 0 ; i < buf.size() ; ++i )
	{
		const int c = buf[i];
		cout << " 0x" << c;
	}
	cout << dec << '\n';

	COBS::encode(buf);

	cout << "encoded data: \n" << hex;
	for( unsigned int i = 0 ; i < buf.size() ; ++i )
	{
		const int c = buf[i];
		cout << " 0x" << c;
	}
	cout << dec << '\n';

	COBS::decode(buf);

	cout << "decoded data: \n" << hex;
	for( unsigned int i = 0 ; i < buf.size() ; ++i )
	{
		const int c = buf[i];
		cout << " 0x" << c;
	}
	cout << dec << '\n';
	return 0;
}
