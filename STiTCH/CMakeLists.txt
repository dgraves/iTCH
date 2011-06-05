set(SERVER_QOBJECT_HEADER_FILES
    STiTCHDialog.h
)

set(SERVER_SOURCE_FILES
    STiTCHDialog.cpp
    main.cpp
)

set(SERVER_FORM_FILES
    STiTCHDialog.ui
)

set(SERVER_RESOURCE_FILES
    STiTCH.qrc
)

set(SERVER_APPLICATION_ICON_FILE
    STiTCH.res
)

# Execute Qt code generation tools
qt4_wrap_cpp(SERVER_QOBJECT_HEADER_FILES_MOC ${SERVER_QOBJECT_HEADER_FILES})
qt4_wrap_ui(SERVER_FORM_HEADER_FILES ${SERVER_FORM_FILES})
qt4_add_resources(SERVER_RESOURCE_FILES_RCC ${SERVER_RESOURCE_FILES})

# Make sure Qt generated files are found
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Add the iTCHCore include path
include_directories(../iTCHCore/include)

add_executable(STiTCH WIN32 ${SERVER_SOURCE_FILES}
                      ${SERVER_QOBJECT_HEADER_FILES}
                      ${SERVER_QOBJECT_HEADER_FILES_MOC}
                      ${SERVER_FORM_HEADER_FILES}
                      ${SERVER_RESOURCE_FILES_RCC}
                      ${SERVER_APPLICATION_ICON_FILE}
)

# Add the Qt libraries as link dependencies
target_link_libraries(STiTCH iTCHCore iTCHCoreCOM ${QT_LIBRARIES} ${PROTOBUF_LIBRARY})

# Use main instead of WinMain for VisualStudio
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif(MSVC)
