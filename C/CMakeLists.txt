CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(libsocket_src
./inet/libinetsocket.c
./unix/libunixsocket.c)

ADD_LIBRARY(socket MODULE ${libsocket_src})

INSTALL(TARGETS socket DESTINATION ${LIB_DIR})
