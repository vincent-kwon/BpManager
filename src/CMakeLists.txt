CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(fw_name "bp-manager")

PROJECT(${fw_name})

SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})

SET(INC_DIR ${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${INC_DIR})

SET(requires "dlog eina glib-2.0 rpc-port ecore capi-base-common dbus-1")
SET(pc_requires "capi-base-common")

INCLUDE(FindPkgConfig)
pkg_check_modules(${fw_name} REQUIRED ${requires})
FOREACH(flag ${${fw_name}_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC ")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -Werror -Wno-error=deprecated-declarations")

IF("${ARCH}" STREQUAL "arm")
    ADD_DEFINITIONS("-DTARGET")
ENDIF("${ARCH}" STREQUAL "arm")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
ADD_DEFINITIONS("-DSLP_DEBUG")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,--rpath=${LIB_INSTALL_DIR}")

aux_source_directory (. SOURCES)

ADD_EXECUTABLE(${fw_name} ${SOURCES})

# TODO(vincent): Why need it?
TARGET_LINK_LIBRARIES(${fw_name} ${${fw_name}_LDFLAGS})

INSTALL(TARGETS ${fw_name} DESTINATION ${BIN_INSTALL_DIR})

CONFIGURE_FILE(org.tizen.bp.manager.conf.in org.tizen.bp.manager.conf @ONLY)
INSTALL(FILES org.tizen.bp.manager.conf DESTINATION ${SYSCONF_INSTALL_DIR}/dbus-1/system.d/)

CONFIGURE_FILE(${DBUS_INTERFACE}.service.in ${DBUS_INTERFACE}.service @ONLY)
INSTALL(FILES ${DBUS_INTERFACE}.service DESTINATION ${SHARE_INSTALL_PREFIX}/dbus-1/system-services/)