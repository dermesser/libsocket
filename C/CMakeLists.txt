CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(libsocket_src
./inet/libinetsocket.c
./unix/libunixsocket.c
)

IF ( NOT IS_SUNOS )
    ADD_DEFINITIONS(-fPIC) # for the static library which needs to be linked into the shared libsocket++.so object.
    ADD_LIBRARY(socket_o OBJECT ${libsocket_src})

    ADD_LIBRARY(socket_int STATIC $<TARGET_OBJECTS:socket_o>) #Static lib must be built for linking libsocket++

    IF(BUILD_SHARED_LIBS)
    ADD_LIBRARY(socket SHARED $<TARGET_OBJECTS:socket_o>)

    INSTALL(TARGETS socket DESTINATION ${LIB_DIR})
    ENDIF()


    IF(BUILD_STATIC_LIBS)
        SET_TARGET_PROPERTIES(socket_int PROPERTIES OUTPUT_NAME socket)
        INSTALL(TARGETS socket_int DESTINATION ${LIB_DIR})
    ENDIF()
ELSE() # On SunOS (e.g. OpenIndiana) we have to link against the system's libsocket. The library is renamed to libsocket_hl (hl = high-level)
    ADD_LIBRARY(socket_hl SHARED ${libsocket_src})
    FIND_LIBRARY(solaris_socket_lib socket)
    TARGET_LINK_LIBRARIES(socket_hl ${solaris_socket_lib})
    INSTALL(TARGETS socket_hl DESTINATION ${LIB_DIR})
ENDIF()

