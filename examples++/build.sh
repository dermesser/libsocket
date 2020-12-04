#!/bin/bash

g++ -lsocket++ -o dgram_over_stream dgram_over_stream.cpp
g++ -lsocket++ -o framing framing.cpp
g++ -lsocket++ -o unix_dgram_syslogclient unix_dgram_syslogclient.cpp

