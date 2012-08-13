.SUFFIXES: .cpp .c .o .so

OBJECTS=libinetsocket.o libunixsocket.o
SHARED_OBJECT=libsocket.so

LIBPATH=/usr/lib

CC=gcc
CPP=g++
# -DVERBOSE to enable verbosity from libinetsocket/libunixsocket
CFLAGS=

.c.o:
	$(CC) -fPIC -c $<

so: $(OBJECTS)
	$(CC) -shared -o ../$(SHARED_OBJECT) $(OBJECTS)

install: so
	install ../$(SHARED_OBJECT) $(LIBPATH) ; \
	ldconfig

obj: $(OBJECTS)

clean:
	rm -f *.o
