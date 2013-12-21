#!/bin/bash

# Part of libsocket
# License: See /LICENSE
#
# Purpose: Test the example programs automatically.
# Normal output is directed to /dev/null so errors
# appear on the screen without the other output.

LIBPATH=/usr/lib
HEADERPATH=/usr/include
CPPFLAGS="-std=c++11 -DVERBOSE"

echo "=== MAKE SURE YOU INSTALLED THE LATEST VERSION AS S/O! ==="
echo "Starting test suite examples++/..."

### TCP client/server
echo "Testing TCP client/server..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o srv -lsocket++ server.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl  -lsocket++ client.cpp

./srv > /dev/null &
sleep 1
./cl > /dev/null

kill %1

rm srv cl

### echo client/server
echo "Testing echo client/server..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl_conn -lsocket++ echo_client_conn.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl_sndto -lsocket++ echo_client_sndto.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o srv -lsocket++ echo_server.cpp

./srv > /dev/null &
sleep 1
./cl_sndto > /dev/null
./cl_conn > /dev/null

kill %1

rm srv cl_sndto cl_conn

### HTTP clients
echo "Testing HTTP clients..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o http1 -lsocket++ http.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o http2 -lsocket++ http_2.cpp

./http1 > /dev/null
./http2 > /dev/null

rm http1 http2

### UNIX dgram client (log client)
echo "Testing UNIX dgram client (log client)..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl -lsocket++ unix_dgram_syslogclient.cpp

./cl > /dev/null

rm cl

### UNIX dgram client/server
echo "Testing UNIX dgram client/server..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o srv -lsocket++ unix_dgram_server.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl -lsocket++ unix_dgram_client.cpp

./srv > /dev/null &
sleep 1
./cl > /dev/null

kill %1

rm cl srv

### UNIX stream server/client
echo "Testing UNIX STREAM client/server..."

g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o srv -lsocket++ unix_server_stream.cpp
g++ $CPPFLAGS -I$HEADERPATH -L$LIBPATH -o cl -lsocket++ unix_client_stream.cpp

./srv > /dev/null &
sleep 1
./cl > /dev/null

kill %1

rm srv cl
