CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(libsocket_src
./inet/libinetsocket.c
./unix/libunixsocket.c)

IF ( NOT IS_SUNOS )
    ADD_LIBRARY(socket MODULE ${libsocket_src})
    INSTALL(TARGETS socket DESTINATION ${LIB_DIR})
ELSE() # On SunOS (e.g. OpenIndiana) we have to link against the system's libsocket. The library is renamed to libsocket_hl (hl = high-level)
    ADD_LIBRARY(socket_hl MODULE ${libsocket_src})
    FIND_LIBRARY(solaris_socket_lib socket)
    TARGET_LINK_LIBRARIES(socket_hl ${solaris_socket})
    INSTALL(TARGETS socket_hl DESTINATION ${LIB_DIR})
ENDIF()

