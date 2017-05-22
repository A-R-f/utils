#include <CircBuf.hpp>
#include <iostream>

using namespace std;

int main()
{
	CircBuf<16> cbuf;
	while(1)
	{
		int d;
		cout << "data: ";
		cin >> d;
		if( d < 0 )
		{
			CircBuf<>::data_type a;
			cout << "buffer has " << cbuf.count() << " elements: ";
			while( cbuf.get(a) == cbuf.OK ) { cout << int(a) << ' '; }
			cout << endl;
		}
		else
		{
			cbuf.put(d);
		}
	}
	return 0;
}
