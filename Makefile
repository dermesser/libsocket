LIBPATH=/usr/lib

all: libsocket libsocketpp

libsocket:
	cd C; \
	make so

libsocketpp:
	cd C++; \
	make so

install:
	cd C; make install; \
	cd ../C++; make install

clean:
	rm *.so; \
	cd C; make clean; \
	cd ../C++; make clean
