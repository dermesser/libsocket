PROJECT(libsocket)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Header conf
SET(VERSION 2.4)

IF( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    SET(IS_FREEBSD 0)
    SET(IS_LINUX 1)
    SET(IS_SUNOS 0)
ELSEIF( CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" )
    SET(IS_FREEBSD 1)
    SET(IS_LINUX 0)
    SET(IS_SUNOS 0)
ELSEIF( CMAKE_SYSTEM_NAME STREQUAL "SunOS" )
    SET(IS_FREEBSD 0)
    SET(IS_LINUX 0)
    SET(IS_SUNOS 1)
ENDIF()

CONFIGURE_FILE(headers/conf.h.in ${CMAKE_CURRENT_BINARY_DIR}/headers/conf.h)

# Compiler configuration
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/headers/ ${CMAKE_CURRENT_SOURCE_DIR}/headers/)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_DEFINITIONS(-Wall -Wextra)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "/usr")
ENDIF()

# Install directories
SET(HEADER_DIR "include/libsocket")
SET(LIB_DIR "lib")

ADD_SUBDIRECTORY(C/)
ADD_SUBDIRECTORY(headers/)

IF( NOT IS_SUNOS )

    ADD_SUBDIRECTORY(C++/)
    ADD_DEPENDENCIES(socket++ socket)

ENDIF()

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
	SET(CPACK_BINARY_DEB "ON")
	SET(CPACK_BINARY_RPM "OFF")
	SET(CPACK_BINARY_STGZ "OFF")
	SET(CPACK_BINARY_TBZ2 "OFF")
	SET(CPACK_BINARY_TGZ "OFF")
	SET(CPACK_BINARY_TZ "OFF")

	SET(CPACK_PACKAGE_NAME "libsocket")
	SET(CPACK_PACKAGE_VERSION "2.4.0")
	SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "")
	SET(CPACK_PACKAGE_CONTACT "Lewin Bormann <lbo@spheniscida.de>")
	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The ultimate socket abstraction library for C and C++.")

	# List executables
	INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_CMAKE_DIR lib/CMake/libsocket)

foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

export(TARGETS socket++ socket_int
  FILE "${PROJECT_BINARY_DIR}/libsocketTargets.cmake")

export(PACKAGE libsocket)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/headers")

configure_file(libsocketConfig.cmake.in
  "${PROJECT_BINARY_DIR}/libsocketConfig.cmake" @ONLY)

configure_file(libsocketConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/libsocketConfigVersion.cmake" @ONLY)

