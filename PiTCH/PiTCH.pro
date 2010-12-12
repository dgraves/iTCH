# -------------------------------------------------
# Project created by QtCreator 2009-07-02T22:34:21
# -------------------------------------------------
QT += network
TARGET = PiTCH
TEMPLATE = app
SOURCES += main.cpp \
    PiTCHWindow.cpp \
    ../iTCHCore/src/iTCH/Method.cpp \
    ../iTCHCore/src/iTCH/Client.cpp \
    PiTCHNetworkDialog.cpp
HEADERS += PiTCHWindow.h \
    ../iTCHCore/include/iTCH/Client.h \
    ../iTCHCore/include/iTCH/Method.h \
    PiTCHNetworkDialog.h \
    ../iTCHCore/include/iTCH/NetworkInfo.h
FORMS += PiTCHWindow.ui \
    PiTCHNetworkDialog.ui
INCLUDEPATH += ../iTCHCore/include
