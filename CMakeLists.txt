cmake_minimum_required(VERSION 2.6.4)

project(iTCH)

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
