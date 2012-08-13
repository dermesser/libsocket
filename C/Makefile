.SUFFIXES: .cpp .c .o .so

CC=gcc
CPP=g++

LIBPATH=/usr/lib

OBJECTS=libinetsocket.o libunixsocket.o
SHARED_OBJECT=libsocket.so

.c.o:
	$(CC) -fPIC -c $<

so: $(OBJECTS)
	$(CC) -shared -o ../$(SHARED_OBJECT) $(OBJECTS)

install: so
	install ../$(SHARED_OBJECT) $(LIBPATH)

obj: $(OBJECTS)

clean:
	rm -f *.o
