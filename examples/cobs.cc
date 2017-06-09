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
	buf.push_back(0);
	buf.insert(buf.begin(), 0);

	COBS::encode(buf.data(), buf.size() - 2);

	cout << "encoded data: \n" << hex;
	for( unsigned int i = 0 ; i < buf.size() ; ++i )
	{
		const int c = buf[i];
		cout << " 0x" << c;
	}
	cout << dec << '\n';

	const int len = COBS::decode(buf.data(), buf.size());

	while( int(buf.size()) > len ) { buf.pop_back(); }

	cout << "decoded data: \n" << hex;
	for( int i = 0 ; i < len ; ++i )
	{
		const int c = buf[i];
		cout << " 0x" << c;
	}
	cout << dec << '\n';
	return 0;
}
