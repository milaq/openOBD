
# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


# Our project is called 'openOBD' this is how it will be called in
# visual studio, and in our makefiles.
project(openobd)
add_subdirectory (po)

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set(PACKAGE_NAME openobd)
include( ${CMAKE_SOURCE_DIR}/VERSION.cmake )
set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
message(STATUS "*** Building ${PROJECT} ${PACKAGE_VERSION} ***")

# Required packages and programs
find_package(wxWidgets COMPONENTS core base richtext html xml adv aui REQUIRED)
find_package(Gettext REQUIRED)
if (UNIX)
	find_package(Sqlite3 REQUIRED)
else (UNIX)
	include_directories(${CMAKE_SOURCE_DIR}/sqlite)
endif (UNIX)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

# wxWidgets include (this will do all the magic to configure everything)
include( ${wxWidgets_USE_FILE} )

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
configure_file("${PROJECT_SOURCE_DIR}/include/header.h.in"
  		"${PROJECT_SOURCE_DIR}/include/header.h")

# For convenience we define the sources as a variable. You can add
# header files and cpp/c files and CMake will sort them out
if (UNIX)
SET(SRC_CTB
    src/ctb-0.15/serportx.cpp
    src/ctb-0.15/fifo.cpp
    src/ctb-0.15/iobase.cpp
    src/ctb-0.15/linux/timer.cpp
    src/ctb-0.15/linux/serport.cpp
)
else (UNIX)
SET(SRC_CTB
    src/ctb-0.15/serportx.cpp
    src/ctb-0.15/fifo.cpp
    src/ctb-0.15/iobase.cpp
    src/ctb-0.15/win32/timer.cpp
    src/ctb-0.15/win32/serport.cpp
)

SET(SRC_SQLITE
    sqlite/sqlite3.c
)
ADD_LIBRARY(SQLITE ${SRC_SQLITE})
endif (UNIX)
ADD_LIBRARY(CTB ${SRC_CTB})

set(SRCS
    src/main.cpp
    src/gui.cpp
    src/obdFrame.cpp
    src/milPanel.cpp
    src/logPanel.cpp
    src/obdbase.cpp
    src/elm327.cpp
    src/dlgOptions.cpp
    src/pidPanel.cpp
)

# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(SRCS ${SRCS} src/${PACKAGE_NAME}.rc)
endif(WIN32)

add_executable(${PACKAGE_NAME} WIN32 ${SRCS})

target_link_libraries(${PACKAGE_NAME} ${wxWidgets_LIBRARIES} CTB)

if (UNIX)
    target_link_libraries(${PACKAGE_NAME} ${Sqlite3_LIBRARY} ${GETTEXT_LIBRARY})
else (UNIX)
    target_link_libraries(${PACKAGE_NAME} SQLITE)
endif (UNIX)

# install
IF (PREFIX)
	SET(CMAKE_INSTALL_PREFIX ${PREFIX})
ENDIF (PREFIX)
SET(PREFIX_BIN bin)
SET(PREFIX_DATA share)
if (UNIX)
    SET(PREFIX_PKGDATA ${PREFIX_DATA}/${PACKAGE_NAME})
    set(PREFIX_DOCS ${PREFIX_DATA}/doc/${PACKAGE_NAME})
else (UNIX)
    SET(PREFIX_PKGDATA data)
    set(PREFIX_DOCS docs)
endif(UNIX)

IF(WIN32)
	INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION ".")
ELSE(WIN32)
	INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION ${PREFIX_BIN})
ENDIF(WIN32)
INSTALL(FILES data/openobd.db DESTINATION ${PREFIX_PKGDATA})
INSTALL(FILES data/license.txt README DESTINATION ${PREFIX_DOCS})

# packaging
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openOBD: OBD-II interface control")
SET(CPACK_PACKAGE_DESCRIPTION "OBD-II communications")
SET(CPACK_PACKAGE_VENDOR "Simon Booth")
SET(CPACK_PACKAGE_CONTACT "Simon Booth <simesb@users.sourceforge.net>")
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
SET(CPACK_SOURCE_IGNORE_FILES "/.git;/build;/po/openobd.pot;/Output;/sqlite;/docs")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/data/installlicense.txt")

# linux specific packaging
IF(UNIX)
    SET(CPACK_GENERATOR "DEB;RPM")
	SET(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
	SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk2.8-0 (>= 2.8.7.1), libsqlite3-0 (>= 3.6)")
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    SET(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "Miscellaneous")
    SET(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
    SET(CPACK_RPM_PACKAGE_ARCHITECTURE  "i386")
    SET(CPACK_RPM_PACKAGE_REQUIRES  "wxGTK gettext sqlite3")
    SET(CPACK_SET_DESTDIR ON)
    SET(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
	SET(CPACK_STRIP_FILES "${PREFIX_BIN}/${PACKAGE_NAME}")
ENDIF(UNIX)

# windows specific packaging
IF(WIN32)
	SET(CPACK_GENERATOR "NSIS")
	SET(CPACK_PACKAGE_EXECUTABLES ${PACKAGE_NAME} "openOBD")
	SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-setup")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PACKAGE_NAME}")
ENDIF(WIN32)

# leave this at the end or it fucks up
INCLUDE(CPack)

