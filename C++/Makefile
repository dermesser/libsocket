.SUFFIXES: .cpp .c .o .so

OBJECTS=dgramclient.o inetbase.o inetclientstream.o inetserverdgram.o socket.o unixbase.o unixclientstream.o unixserverdgram.o \
exception.o inetclientdgram.o inetdgram.o inetserverstream.o select.o streamclient.o unixclientdgram.o unixdgram.o unixserverstream.o

LIBPATH=/usr/lib

SHARED_OBJECT=libsocket++.so

LIBSOCKET_STATIC=libsocket.a
LIBSOCKETPP_STATIC=libsocket++.a
API_PARENT_INC_DIR=$(CURDIR)/..

CXX=g++
# CXXFLAGS for C++/*.cpp files
CXXFLAGS+=-I $(API_PARENT_INC_DIR)
override CXXFLAGS+=-std=c++11

# _MIXED enables some "extern" declarations in the C headers
.cpp.o:
	$(CXX) -D_MIXED $(CXXFLAGS) -fPIC -c $<

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
	$(CXX) $(CXXFLAGS) -shared -o ../$(SHARED_OBJECT) -Wl,--whole-archive ../$(LIBSOCKET_STATIC) ../$(LIBSOCKETPP_STATIC) -Wl,--no-whole-archive


# Install SO
install: so
	install ../$(SHARED_OBJECT) $(LIBPATH) ; \
	ldconfig

# Clean this directory and the libsocket directory
clean:
	rm -f *.o ; \
	cd ../C/ ; \
	$(MAKE) clean
