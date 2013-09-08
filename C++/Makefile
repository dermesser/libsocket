.SUFFIXES: .cpp .c .o .so

OBJECTS=dgramclient.o inetbase.o inetclientstream.o inetserverdgram.o socket.o unixbase.o unixclientstream.o unixserverdgram.o \
exception.o inetclientdgram.o inetdgram.o inetserverstream.o select.o streamclient.o unixclientdgram.o unixdgram.o unixserverstream.o

LIBPATH=/usr/lib

SHARED_OBJECT=libsocket++.so

LIBSOCKET_STATIC=libsocket.a
LIBSOCKETPP_STATIC=libsocket++.a

CC=gcc
CPP=g++
# CPPFLAGS for C++/*.cpp files
CPPFLAGS= -std=c++11

# _MIXED enables some "extern" declarations in the C headers
.cpp.o:
	$(CPP) -D_MIXED $(CPPFLAGS) -fPIC -c $<

# Build C++ object files
obj: $(OBJECTS)

# Build C object files (libsocket)
libsocket:
	cd ../C/;  \
	$(MAKE) static

# Build static object
static: obj
	ar rs ../$(LIBSOCKETPP_STATIC) $(OBJECTS)

# Build shared object
so: static libsocket
	$(CPP) $(CPPFLAGS) -shared -o ../$(SHARED_OBJECT) -Wl,--whole-archive ../$(LIBSOCKET_STATIC) ../$(LIBSOCKETPP_STATIC) -Wl,--no-whole-archive


# Install SO
install: so
	install ../$(SHARED_OBJECT) $(LIBPATH) ; \
	ldconfig

# Clean this directory and the libsocket directory
clean:
	rm -f *.o ; \
	cd ../C/ ; \
	$(MAKE) clean
