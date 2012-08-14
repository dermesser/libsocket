LIBPATH=/usr/lib
HEADERPATH=/usr/include/libsocket

CC=gcc
CPP=g++
CFLAGS=""

all: libsocket libsocketpp

libsocket:
	cd C; \
	$(MAKE) so

libsocketpp:
	cd C++; \
	$(MAKE) so

install: install-headers
	cd C; $(MAKE) install; \
	cd ../C++; $(MAKE) install

install-headers:
	mkdir -p $(HEADERPATH); \
	cp headers/*.h* $(HEADERPATH)

clean:
	rm *.so; \
	cd C; $(MAKE) clean; \
	cd ../C++; $(MAKE) clean

deinstall:
	rm $(LIBPATH)/libsocket.so $(LIBPATH)/libsocket++.so ; \
	rm -r $(HEADERPATH)
