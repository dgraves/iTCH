# -------------------------------------------------
# Project created by QtCreator 2009-07-09T00:38:12
# -------------------------------------------------
QT += network
TARGET = STiTCH
TEMPLATE = app
SOURCES += main.cpp \
    STiTCHDialog.cpp \
    ../iTCHCore/src/iTCH/Method.cpp \
    ../iTCHCore/src/iTCH/Server.cpp \
    ../iTCHCore/src/iTCH/Controller.cpp \
    ../iTCHCore/src/iTCH/Connection.cpp \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface_i.c \
    ../iTCHCore/src/iTCH/EventSink.cpp
HEADERS += STiTCHDialog.h \
    ../iTCHCore/include/iTCH/Method.h \
    ../iTCHCore/include/iTCH/Server.h \
    ../iTCHCore/include/iTCH/Controller.h \
    ../iTCHCore/include/iTCH/Connection.h \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface.h \
    ../iTCHCore/include/iTCH/EventSink.h
FORMS += STiTCHDialog.ui
INCLUDEPATH += ../iTCHCore/include \
    ../iTunesCOMWindowsSDK \
    C:/Development/protobuf-2.3.0/src
LIBS += ole32.lib oleaut32.lib
RESOURCES += STiTCH.qrc
