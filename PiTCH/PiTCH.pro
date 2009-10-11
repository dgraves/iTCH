# -------------------------------------------------
# Project created by QtCreator 2009-07-02T22:34:21
# -------------------------------------------------
QT += network
TARGET = PiTCH
TEMPLATE = app
SOURCES += main.cpp \
    PiTCHWindow.cpp \
    ../iTCHCore/iTCHMethod.cpp \
    ../iTCHCore/iTCHClient.cpp \
    PiTCHNetworkDialog.cpp
HEADERS += PiTCHWindow.h \
    ../iTCHCore/iTCHClient.h \
    ../iTCHCore/iTCHMethod.h \
    PiTCHNetworkDialog.h \
    ../iTCHCore/iTCHNetworkInfo.h
FORMS += PiTCHWindow.ui \
    PiTCHNetworkDialog.ui
INCLUDEPATH += ../iTCHCore
