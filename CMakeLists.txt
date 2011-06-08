cmake_minimum_required(VERSION 2.6.4)

project(iTCH)

set(VERSION "0.7.0")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "7")
set(VERSION_PATCH "0")

if(WIN32 AND NOT UNIX)
    set(RUNTIME_INSTALL_DIR .)
else(WIN32 AND NOT UNIX)
    set(RUNTIME_INSTALL_DIR bin)
endif(WIN32 AND NOT UNIX)

# Import the protocol buffer related files
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

# Import Qt 4 related headers, libraries, and tools
find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Build the core library for the iTunes changer client and server apps
add_subdirectory(iTCHCore)

# Select optional programs to build: server/iTunes plug-in and client
option(BUILD_CLIENT "Build PiTCH client application" TRUE)
if(BUILD_CLIENT)
    add_subdirectory(PiTCH)
endif(BUILD_CLIENT)

# Server require iTunes Windows COM SDK and is Windows only
if(WIN32)
    option(BUILD_SERVER "Build STiTCH server application/plug-in for iTunes" TRUE)
    if(BUILD_SERVER)
        add_subdirectory(STiTCH)
    endif(BUILD_SERVER)
endif(WIN32)


# Install setup
if(WIN32 AND NOT UNIX)
	# Add Qt DLLs to install
    INSTALL(FILES
        "${QT_BINARY_DIR}/QtCore${QT_VERSION_MAJOR}.dll"
        "${QT_BINARY_DIR}/QtGui${QT_VERSION_MAJOR}.dll"
        "${QT_BINARY_DIR}/QtNetwork${QT_VERSION_MAJOR}.dll"
        DESTINATION ${RUNTIME_INSTALL_DIR}
    )

    # Don't put required system libraries in the 'bin' subdir
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${RUNTIME_INSTALL_DIR})
endif(WIN32 AND NOT UNIX)

include(InstallRequiredSystemLibraries)


# CPack setup
if(WIN32 AND NOT UNIX)
    # Select a CPack genetator based on platform
    set(CPACK_GENERATOR "NSIS")
endif(WIN32 AND NOT UNIX)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "iTCH - iTCH applications for remotely controlling Apple's iTunes software")
set(CPACK_PACKAGE_VENDOR "dgraves.org")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Deployment/About.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Deployment/License.txt")
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "iTCH")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "iTCH-${CPACK_PACKAGE_VERSION}")

set(CPACK_PACKAGE_CONTACT "dgraves@computer.org")

set(CPACK_PACKAGE_NAME "iTCH")

if(NOT DEFINED CPACK_SYSTEM_NAME)
    # make sure package is not Cygwin-unknown, for Cygwin just
    # cygwin is good for the system name
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
        set(CPACK_SYSTEM_NAME Cygwin)
    else("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
        set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
    endif("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
endif(NOT DEFINED CPACK_SYSTEM_NAME)

if(${CPACK_SYSTEM_NAME} MATCHES Windows)
    if(CMAKE_CL_64)
        set(CPACK_SYSTEM_NAME win64)
    else(CMAKE_CL_64)
        set(CPACK_SYSTEM_NAME win32)
    endif(CMAKE_CL_64)
endif(${CPACK_SYSTEM_NAME} MATCHES Windows)

if(NOT DEFINED CPACK_PACKAGE_FILE_NAME)
    if(CYGWIN)
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_SOURCE_PACKAGE_FILE_NAME}")
    else(CYGWIN)
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_SOURCE_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME}")
    endif(CYGWIN)
endif(NOT DEFINED CPACK_PACKAGE_FILE_NAME)

if(UNIX)
    set(CPACK_STRIP_FILES "${RUNTIME_INSTALL_DIR}/pitch")
    set(CPACK_SOURCE_STRIP_FILES "")
    set(CPACK_PACKAGE_EXECUTABLES "pitch" "PiTCH")
elseif(WIN32)
    # STiTCH is only available for Windows
    set(CPACK_PACKAGE_EXECUTABLES "pitch" "PiTCH" "stitch" "STiTCH")
endif(UNIX)

if(CPACK_GENERATOR MATCHES "NSIS")
    if(CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
        set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} (Win64)")
        set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION} (Win64)")
    else(CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
        set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
        set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}")
    endif(CMAKE_CL_64)

    # set the install/unistall icon used for the installer itself
    # There is a bug in NSI that does not handle full unix paths properly.
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Deployment\\iTCH.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/Deployment\\iTCH.ico")

    # set the package header icon for MUI
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Deployment\\iTCHInstall.bmp")

    # Use the icon from cmake-gui for add-remove programs
    if(BUILD_CLIENT)
        set(CPACK_NSIS_INSTALLED_ICON_NAME "pitch.exe")
    elseif(BUILD_SERVER)
        set(CPACK_NSIS_INSTALLED_ICON_NAME "stitch.exe")
    endif(BUILD_CLIENT)

    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_DIR})

    set(CPACK_NSIS_PACKAGE_NAME "iTCH")
    set(CPACK_NSIS_DISPLAY_NAME "iTCH")
    set(CPACK_NSIS_HELP_LINK "https://github.com/dgraves/iTCH/issues")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://www.dgraves.org")
    set(CPACK_NSIS_CONTACT ${CPACK_PACKAGE_CONTACT})
endif(CPACK_GENERATOR MATCHES "NSIS")

if(CPACK_GENERATOR MATCHES "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dustin Graves") #required
endif(CPACK_GENERATOR MATCHES "DEB")


# CPack component setup
if(BUILD_CLIENT)
    set(CPACK_COMPONENT_PITCH_DISPLAY_NAME "PiTCH Application")
    set(CPACK_COMPONENT_PITCH_DESCRIPTION
        "A client application for the STiTCH plug-in for iTunes, which can send commands to the STiTCH plug-in to control iTunes remotely."  
    )
endif(BUILD_CLIENT)

if(BUILD_SERVER)
    set(CPACK_COMPONENT_STITCH_DISPLAY_NAME "STiTCH Plug-in")
    set(CPACK_COMPONENT_STITCH_DESCRIPTION
        "A plug-in for iTunes, which allows the iTunes player to be remotely controlled with the PiTCH application."
    )
endif(BUILD_SERVER)

include(CPack)
