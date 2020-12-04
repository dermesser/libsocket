#!/bin/bash

gcc -lsocket -o srv unix_dgram_server.c
gcc -lsocket -o cl  unix_dgram_client.c
gcc -lsocket -o connect_cl  unix_dgram_connect_client.c

