#!/bin/bash

g++ -lsocket++ -o cl  unix_client_stream.cpp
g++ -lsocket++ -o srv unix_server_stream.cpp
