#!/bin/bash

gcc -lsocket -o srv transmission_server.c
gcc -lsocket -o cl  transmission_client.c

