#!/bin/bash

gcc -lsocket -o srv echo_dgram_server.c
gcc -lsocket -o cl  echo_dgram_client.c
gcc -lsocket -o connect_cl  echo_dgram_connect_client.c

