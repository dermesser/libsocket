#!/bin/bash

gcc -lsocket -o http http.c
gcc -lsocket -o get_address_family get_address_family.c
gcc -lsocket -o multicast_listen multicast_listen.c

