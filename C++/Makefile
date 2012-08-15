.SUFFIXES: .cpp .c .o .so

OBJECTS=dgramclient.o inetbase.o inetclientstream.o inetserverdgram.o socket.o unixbase.o unixclientstream.o unixserverdgram.o \
exception.o inetclientdgram.o inetdgram.o inetserverstream.o select.o streamclient.o unixclientdgram.o unixdgram.o unixserverstream.o

LIBPATH=/usr/lib

SHARED_OBJECT=libsocket++.so

CC=gcc
CPP=g++
# CPPFLAGS for C++/*.cpp files
CPPFLAGS=

.cpp.o:
	$(CPP) -D_MIXED $(CPPFLAGS) -fPIC -c $<

# Build C++ object files
obj: $(OBJECTS)

# Build C object files (libsocket)
libsocket:
	cd ../C/;  \
	$(MAKE) obj

# Build Shared Object
so: obj libsocket
	$(CPP) $(CPPFLAGS) -shared -o ../$(SHARED_OBJECT) ../C/*.o $(OBJECTS)

# Install SO
install: so
	install ../$(SHARED_OBJECT) $(LIBPATH) ; \
	ldconfig

# Clean this directory and the libsocket directory
clean:
	rm -f *.o ; \
	cd ../C/ ; \
	$(MAKE) clean
