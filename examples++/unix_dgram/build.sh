#!/bin/bash

g++ -lsocket++ -o srv unix_dgram_server.cpp
g++ -lsocket++ -o cl unix_dgram_client.cpp
