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

ADD_DEFINITIONS(-fPIC) # for the static library which needs to be linked into the shared libsocket++.so object.
ADD_LIBRARY(socket++_o OBJECT ${sources})

IF(BUILD_SHARED_LIBS)
ADD_LIBRARY(socket++ SHARED $<TARGET_OBJECTS:socket++_o>)

TARGET_LINK_LIBRARIES(socket++ socket_int)

INSTALL(TARGETS socket++ DESTINATION ${LIB_DIR})
ENDIF()

IF(BUILD_STATIC_LIBS)
ADD_LIBRARY(socket++_int STATIC $<TARGET_OBJECTS:socket++_o>)

SET_TARGET_PROPERTIES(socket++_int PROPERTIES OUTPUT_NAME socket++)

TARGET_LINK_LIBRARIES(socket++_int socket_int)

INSTALL(TARGETS socket++_int DESTINATION ${LIB_DIR})
ENDIF()