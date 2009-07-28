# -------------------------------------------------
# Project created by QtCreator 2009-07-09T00:38:12
# -------------------------------------------------
QT += network
TARGET = STiTCH
TEMPLATE = app
SOURCES += main.cpp \
    STiTCHDialog.cpp \
    ../common/iTCHMethod.cpp \
    ../common/iTCHServer.cpp \
    ../common/iTCHController.cpp \
    ../common/iTCHConnection.cpp \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface_i.c \
    ../common/iTCHEventSink.cpp
HEADERS += STiTCHDialog.h \
    ../common/iTCHMethod.h \
    ../common/iTCHServer.h \
    ../common/iTCHController.h \
    ../common/iTCHConnection.h \
    ../iTunesCOMWindowsSDK/iTunesCOMInterface.h \
    ../common/iTCHEventSink.h
FORMS += STiTCHDialog.ui
INCLUDEPATH += ../common \
    ../iTunesCOMWindowsSDK
LIBS += ole32.lib oleaut32.lib
RESOURCES += STiTCH.qrc
