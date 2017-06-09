all: circbuf cobs pipe serial signal thread

clean:
	rm -rf circbuf pipe serial signal thread

circbuf: circbuf.cc ../include/CircBuf.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include circbuf.cc -o circbuf

cobs: cobs.cc ../include/COBS.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include cobs.cc -o cobs

pipe: pipe.cc ../include/linux/Pipe.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include pipe.cc -o pipe

serial: serial.cc ../include/linux/Serial.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include serial.cc -o serial

signal: signal.cc ../include/linux/Signal.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include signal.cc -o signal

thread: thread.cc ../include/linux/Thread.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include thread.cc -o thread -lpthread
