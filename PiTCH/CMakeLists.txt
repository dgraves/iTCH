set(CLIENT_QOBJECT_HEADER_FILES
    PiTCHNetworkDialog.h
    PiTCHWindow.h
)

set(CLIENT_SOURCE_FILES
    PiTCHNetworkDialog.cpp
    PiTCHWindow.cpp
    main.cpp
)

set(CLIENT_FORM_FILES
    PiTCHNetworkDialog.ui
    PiTCHWindow.ui
)

set(CLIENT_RESOURCE_FILES
    PiTCH.qrc
)

# Execute Qt code generation tools
qt4_wrap_cpp(CLIENT_QOBJECT_HEADER_FILES_MOC ${CLIENT_QOBJECT_HEADER_FILES})
qt4_wrap_ui(CLIENT_FORM_HEADER_FILES ${CLIENT_FORM_FILES})
qt4_add_resources(CLIENT_RESOURCE_FILES_RCC ${CLIENT_RESOURCE_FILES})

# Make sure Qt generated files are found
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Add the iTCHCore include path
include_directories(../iTCHCore/include)

add_executable(PiTCH WIN32 ${CLIENT_SOURCE_FILES}
                     ${CLIENT_QOBJECT_HEADER_FILES}
                     ${CLIENT_QOBJECT_HEADER_FILES_MOC}
                     ${CLIENT_FORM_HEADER_FILES}
                     ${CLIENT_RESOURCE_FILES_RCC}
)

# Add the Qt libraries as link dependencies
target_link_libraries(PiTCH iTCHCore ${QT_LIBRARIES} ${PROTOBUF_LIBRARY})

# Use main instead of WinMain for VisualStudio
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif(MSVC)
