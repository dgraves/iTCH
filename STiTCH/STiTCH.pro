# -------------------------------------------------
# Project created by QtCreator 2009-07-09T00:38:12
# -------------------------------------------------
QT += network
TARGET = STiTCH
TEMPLATE = app
SOURCES += main.cpp \
    STiTCHDialog.cpp \
    ../iTCHCore/iTCHMethod.cpp \
    ../iTCHCore/iTCHServer.cpp \
    ../iTCHCore/iTCHController.cpp \
    ../iTCHCore/iTCHConnection.cpp \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface_i.c \
    ../iTCHCore/iTCHEventSink.cpp
HEADERS += STiTCHDialog.h \
    ../iTCHCore/iTCHMethod.h \
    ../iTCHCore/iTCHServer.h \
    ../iTCHCore/iTCHController.h \
    ../iTCHCore/iTCHConnection.h \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface.h \
    ../iTCHCore/iTCHEventSink.h
FORMS += STiTCHDialog.ui
INCLUDEPATH += ../iTCHCore \
    ../iTunesCOMWindowsSDK
LIBS += ole32.lib oleaut32.lib
RESOURCES += STiTCH.qrc
