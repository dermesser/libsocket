cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11") # -DVERBOSE")

ADD_DEFINITIONS("-DMIXED")

SET(sources
dgramclient.cpp
dgramoverstream.cpp
framing.cpp
inetbase.cpp
inetclientstream.cpp
inetserverdgram.cpp
select.cpp
streamclient.cpp
unixclientdgram.cpp
unixdgram.cpp
unixserverstream.cpp
exception.cpp
inetclientdgram.cpp
inetdgram.cpp
inetserverstream.cpp
socket.cpp
unixbase.cpp
unixclientstream.cpp
unixserverdgram.cpp
)

ADD_LIBRARY(socket++ SHARED ${sources})

TARGET_LINK_LIBRARIES(socket++ socket_int)

INSTALL(TARGETS socket++ DESTINATION ${LIB_DIR})
