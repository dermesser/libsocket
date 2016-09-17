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
./dgramoverstream.hpp
./framing.hpp
)

IF(IS_LINUX)
    SET(headers ${headers} ./epoll.hpp)
ENDIF()

INSTALL(FILES ${headers} DESTINATION ${HEADER_DIR})
