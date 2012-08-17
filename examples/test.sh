#!/bin/bash

# Part of libsocket
# License: See /LICENSE
#
# Purpose: Test the example programs automatically.
# Normal output is directed to /dev/null so errors
# appear on the screen without the other output.

LIBPATH=/usr/lib
HEADERPATH=/usr/include

echo "=== MAKE SURE YOU INSTALLED THE LATEST VERSION AS S/O! ==="
echo "Starting test suite examples/..."

echo "Testing TCP client/server (transmission*.c)..."

gcc -I$HEADERPATH -L$LIBPATH -o cl -lsocket transmission_client.c
gcc -I$HEADERPATH -L$LIBPATH -o srv -lsocket transmission_server.c

./srv > /dev/null &
sleep 1
./cl > /dev/null

# Server kills itself

rm srv cl

echo "Testing echo UDP programs..."

gcc -I$HEADERPATH -L$LIBPATH -o cl -lsocket echo_dgram_client.c
gcc -I$HEADERPATH -L$LIBPATH -o clc -lsocket echo_dgram_connect_client.c
gcc -I$HEADERPATH -L$LIBPATH -o srv -lsocket echo_dgram_server.c

./srv > /dev/null &
sleep 1
./clc > /dev/null
./cl > /dev/null

killall srv

rm srv clc cl

echo "Testing HTTP client..."

gcc -o http -lsocket http.c

./http > /dev/null

rm http

echo "Testing UNIX dgram client/server..."

gcc -I$HEADERPATH -L$LIBPATH -o cl -lsocket unix_dgram_client.c
gcc -I$HEADERPATH -L$LIBPATH -o clc -lsocket unix_dgram_connected_client.c
gcc -I$HEADERPATH -L$LIBPATH -o srv  -lsocket unix_dgram_server.c

./srv > /dev/null &
sleep 1
./clc > /dev/null
./cl > /dev/null

killall srv

rm srv clc cl

echo "Testing UNIX stream client/server..."

gcc -I$HEADERPATH -L$LIBPATH -o srv -lsocket unix_stream_server.c
gcc -I$HEADERPATH -L$LIBPATH -o cl -lsocket unix_stream_client.c

./srv > /dev/null &
sleep 1
./cl > /dev/null

killall srv

rm cl srv
