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

gcc -I$HEADERPATH -L$LIBPATH -o cl transmission_client.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o srv transmission_server.c -lsocket

./srv > /dev/null &
sleep 1
./cl > /dev/null

# Server kills itself

rm srv cl

echo "Testing echo UDP programs..."

gcc -I$HEADERPATH -L$LIBPATH -o cl echo_dgram_client.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o clc echo_dgram_connect_client.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o srv echo_dgram_server.c -lsocket

./srv > /dev/null &
sleep 1
./clc > /dev/null
./cl > /dev/null

killall srv

rm srv clc cl

echo "Testing HTTP client..."

gcc -o http http.c -lsocket

./http > /dev/null

rm http

echo "Testing UNIX dgram client/server..."

gcc -I$HEADERPATH -L$LIBPATH -o cl unix_dgram_client.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o clc unix_dgram_connected_client.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o srv  unix_dgram_server.c -lsocket

./srv > /dev/null &
sleep 1
./clc > /dev/null
./cl > /dev/null

killall srv

rm srv clc cl

echo "Testing UNIX stream client/server..."

gcc -I$HEADERPATH -L$LIBPATH -o srv unix_stream_server.c -lsocket
gcc -I$HEADERPATH -L$LIBPATH -o cl unix_stream_client.c -lsocket

./srv > /dev/null &
sleep 1
./cl > /dev/null

killall srv

rm cl srv
