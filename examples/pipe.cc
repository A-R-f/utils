#include <iostream>
#include <sstream>
#include <linux/Pipe.hpp>

using namespace std;

int main()
{
	Pipe p;
	char rstr[11];
	rstr[10] = '\0';
	unsigned int di = 1;
	for( unsigned int i = 0 ; i <= 1000 ; i += di )
	{
		ostringstream os;
		os << i << ' ' << "this will be truncated";
		const string& s = os.str();
		cout << s;
		p.write(s.c_str(), s.length());
		cout << "\t-->\t";
		p.read(rstr, 10);
		p.flush();
		cout << rstr;
		cout << endl;
		if( i >= 10 * di ) { di *= 10; }
	}
	return 0;
}
