TARGETS = circbuf cobs pipe serial signal thread tokenizer
WINTARGETS = winserial.exe winthread.exe

all: $(TARGETS)

winall: $(WINTARGETS)

clean:
	rm -rf $(TARGETS) $(WINTARGETS)

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

thread: thread.cc ../include/linux/Thread.hpp ../include/linux/Mutex.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include thread.cc -o thread -lpthread

tokenizer: tokenizer.cc ../include/Tokenizer.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include tokenizer.cc -o tokenizer

winserial.exe: winserial.cc ../include/windows/Serial.hpp
	x86_64-w64-mingw32-g++ -static -O2 -Wall -Wextra -Wpedantic -I../include winserial.cc -o winserial.exe

winthread.exe: winthread.cc ../include/windows/Thread.hpp ../include/windows/Mutex.hpp
	x86_64-w64-mingw32-g++ -static -O2 -Wall -Wextra -Wpedantic -I../include winthread.cc -o winthread.exe
