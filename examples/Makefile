all: circbuf pipe

clean:
	rm -rf circbuf pipe

circbuf: circbuf.cc ../include/CircBuf.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include circbuf.cc -o circbuf

pipe: pipe.cc ../include/linux/Pipe.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include pipe.cc -o pipe
