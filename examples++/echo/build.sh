#!/bin/bash

g++ -lsocket++ -o conn echo_client_conn.cpp
g++ -lsocket++ -o sndto echo_client_sndto.cpp
g++ -lsocket++ -o srv echo_server.cpp
