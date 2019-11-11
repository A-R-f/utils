TARGETS = circbuf cobs daemon i2c_bus module module.so msgproc multipacket pagedmemory pipe runtimefile serial signal socket thread timeout tokenizer
WINTARGETS = winserial.exe winthread.exe

all: $(TARGETS)

winall: $(WINTARGETS)

clean:
	rm -rf $(TARGETS) $(WINTARGETS)

circbuf: circbuf.cc ../include/CircBuf.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include circbuf.cc -o circbuf

cobs: cobs.cc ../include/COBS.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include cobs.cc -o cobs

daemon: daemon.cc ../include/linux/Daemon.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include daemon.cc -o daemon

i2c_bus: i2c_bus.cc ../include/linux/I2C_Bus.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include i2c_bus.cc -o i2c_bus

module: module.cc ../include/linux/Module.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include -DAPP module.cc -o module -ldl

module.so: module.cc ../include/linux/Module.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include -DMOD -fPIC -shared module.cc -o module.so -ldl

msgproc: msgproc.cc ../include/Message_Processor.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include msgproc.cc -o msgproc

multipacket: multipacket.cc ../include/MultiPacket.hpp
	g++ -O2 -Wall -Wextra -Wpedantic -I../include -std=c++11 multipacket.cc -o multipacket

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
