# Add current include path to list
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Core header files to be included with IDE projects (not really needed for build)
set(CORE_HEADER_FILES
    include/iTCH/MessageBuilder.h
    include/iTCH/NetworkInfo.h
)

# Core header files to be processed by MOC
set(CORE_QOBJECT_HEADER_FILES
    include/iTCH/Client.h
    include/iTCH/Connection.h
    include/iTCH/Server.h
)

# Core source files for the iTCHCore library
set(CORE_SOURCE_FILES
    src/iTCH/Client.cpp
    src/iTCH/Connection.cpp
    src/iTCH/MessageBuilder.cpp
    src/iTCH/Server.cpp
)

# Invoke MOC
qt4_wrap_cpp(CORE_QOBJECT_HEADER_FILES_MOC ${CORE_QOBJECT_HEADER_FILES})

# Generate protobuf files
set(CORE_PROTO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)
set(CORE_PROTO_FILE ${CORE_PROTO_PATH}/iTCH.proto)
set(CORE_PROTO_OUT ${CMAKE_CURRENT_SOURCE_DIR}/include/GeneratedCode)
set(CORE_PROTO_GENERATED_FILES "${CORE_PROTO_OUT}/iTCH.pb.cc" "${CORE_PROTO_OUT}/iTCH.pb.h")
add_custom_command(
    OUTPUT ${CORE_PROTO_GENERATED_FILES}
    COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --cpp_out ${CORE_PROTO_OUT} --proto_path ${CORE_PROTO_PATH} ${CORE_PROTO_FILE}
    MAIN_DEPENDENCY ${CORE_PROTO_FILE}
    COMMENT "Running C++ protocol buffer compiler on ${CORE_PROTO_PATH}"
    VERBATIM
)

add_library(iTCHCore STATIC ${CORE_SOURCE_FILES} 
                            ${CORE_HEADER_FILES}
                            ${CORE_QOBJECT_HEADER_FILES}
                            ${CORE_QOBJECT_HEADER_FILES_MOC}
                            ${CORE_PROTO_FILE}
)

#####################################################################

# Setup Windows only iTunes COM SDK dependent library
if(WIN32)
    # Add iTunes COM SDK path to list
    include_directories(${iTCH_SOURCE_DIR}/iTunesCOMWindowsSDK)

    # The Windows only iTunes COM SDK dependent files to be included with IDE projects
    set(CORE_COM_HEADER_FILES
	    include/iTCH/EventSink.h
        ${iTCH_SOURCE_DIR}/iTunesCOMWindowsSDK/iTunesCOMInterface.h
    )

    # The Windows only iTunes COM SDK dependent files to be processed by MOC
    set(CORE_COM_QOBJECT_HEADER_FILES
        include/iTCH/Controller.h
    )

    # The Windows only iTunes COM SDK dependent files for the iTCHCoreCOM library
    set(CORE_COM_SOURCE_FILES
        src/iTCH/Controller.cpp
        src/iTCH/EventSink.cpp
        ${iTCH_SOURCE_DIR}/iTunesCOMWindowsSDK/iTunesCOMInterface_i.c
    )

    # Invoke MOC
    qt4_wrap_cpp(CORE_COM_QOBJECT_HEADER_FILES_MOC ${CORE_COM_QOBJECT_HEADER_FILES})

    add_library(iTCHCoreCOM STATIC ${CORE_COM_SOURCE_FILES} 
                                   ${CORE_COM_HEADER_FILES}
							       ${CORE_COM_QOBJECT_HEADER_FILES}
                                   ${CORE_COM_QOBJECT_HEADER_FILES_MOC}
)
endif(WIN32)
