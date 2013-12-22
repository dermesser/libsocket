CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(headers
./unixdgram.hpp
./exception.hpp
./inetclientdgram.hpp
./libinetsocket.h
./unixserverstream.hpp
./dgramclient.hpp
./streamclient.hpp
./inetserverstream.hpp
./unixclientdgram.hpp
./socket.hpp
./inetbase.hpp
./inetserverdgram.hpp
./unixclientstream.hpp
./libunixsocket.h
./select.hpp
./inetclientstream.hpp
./unixbase.hpp
./unixserverdgram.hpp
./inetdgram.hpp
)

SET(excluded
./config.h
./config.h.in
)

SET(HEADER_DIR /usr/include/libsocket)

INSTALL(FILES ${headers} DESTINATION ${HEADER_DIR})
