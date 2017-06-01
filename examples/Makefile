all: circbuf pipe signal thread

clean:
	rm -rf circbuf pipe signal thread

circbuf: circbuf.cc ../include/CircBuf.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include circbuf.cc -o circbuf

pipe: pipe.cc ../include/linux/Pipe.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include pipe.cc -o pipe

signal: signal.cc ../include/linux/Signal.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include signal.cc -o signal

thread: thread.cc ../include/linux/Thread.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include thread.cc -o thread -lpthread
