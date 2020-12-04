#!/bin/bash

g++ -lsocket++ -o http http.cpp
g++ -lsocket++ -o http_2 http_2.cpp
g++ -lsocket++ -o http_epoll http_epoll.cpp
