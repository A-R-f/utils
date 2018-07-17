TARGETS = circbuf cobs msgproc pagedmemory pipe runtimefile serial signal socket thread timeout tokenizer
WINTARGETS = winserial.exe winthread.exe

all: $(TARGETS)

winall: $(WINTARGETS)

clean:
	rm -rf $(TARGETS) $(WINTARGETS)

circbuf: circbuf.cc ../include/CircBuf.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include circbuf.cc -o circbuf

cobs: cobs.cc ../include/COBS.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include cobs.cc -o cobs

msgproc: msgproc.cc ../include/Message_Processor.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include msgproc.cc -o msgproc

pagedmemory: pagedmemory.cc ../include/PagedMemory.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include pagedmemory.cc -o pagedmemory

pipe: pipe.cc ../include/linux/Pipe.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include pipe.cc -o pipe

runtimefile: runtimefile.cc ../include/RunTimeFile.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include runtimefile.cc -o runtimefile

serial: serial.cc ../include/linux/Serial.hpp ../include/linux/Poll.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include serial.cc -o serial

signal: signal.cc ../include/linux/Signal.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include signal.cc -o signal

socket: socket.cc ../include/linux/Socket.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include socket.cc -o socket

thread: thread.cc ../include/linux/Thread.hpp ../include/linux/Mutex.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include thread.cc -o thread -lpthread

timeout: timeout.cc ../include/linux/Timeout.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include timeout.cc -o timeout

tokenizer: tokenizer.cc ../include/Tokenizer.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include tokenizer.cc -o tokenizer

winserial.exe: winserial.cc ../include/windows/Serial.hpp
	x86_64-w64-mingw32-g++ -static -O2 -Wall -Wextra -Wpedantic -I../include winserial.cc -o winserial.exe

winthread.exe: winthread.cc ../include/windows/Thread.hpp ../include/windows/Mutex.hpp
	x86_64-w64-mingw32-g++ -static -O2 -Wall -Wextra -Wpedantic -I../include winthread.cc -o winthread.exe
