.SUFFIXES: .cpp .c .o .so

CC=gcc
CPP=g++
# _MIXED enables extern "C" linkage
CPPFLAGS=-D_MIXED

LIBPATH=/usr/lib

OBJECTS=dgramclient.o inetbase.o inetclientstream.o inetserverdgram.o socket.o unixbase.o unixclientstream.o unixserverdgram.o \
exception.o inetclientdgram.o inetdgram.o inetserverstream.o select.o streamclient.o unixclientdgram.o unixdgram.o unixserverstream.o

SHARED_OBJECT=libsocket++.so

.cpp.o:
	$(CPP) $(CPPFLAGS) -fPIC -c $<

# Build C++ object files
obj: $(OBJECTS)

# Build C object files (libsocket)
libsocket:
	cd ../C/;  \
	make obj

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
	make clean
