# -------------------------------------------------
# Project created by QtCreator 2009-07-02T22:34:21
# -------------------------------------------------
QT += network
TARGET = PiTCH
TEMPLATE = app
SOURCES += main.cpp \
    PiTCHWindow.cpp \
    ../common/iTCHMethod.cpp \
    ../common/iTCHClient.cpp \
    PiTCHNetworkDialog.cpp
HEADERS += PiTCHWindow.h \
    ../common/iTCHClient.h \
    ../common/iTCHMethod.h \
    PiTCHNetworkDialog.h \
    ../common/iTCHNetworkInfo.h
FORMS += PiTCHWindow.ui \
    PiTCHNetworkDialog.ui
INCLUDEPATH += ../common
