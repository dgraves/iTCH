# -------------------------------------------------
# Project created by QtCreator 2009-07-09T00:38:12
# -------------------------------------------------
QT += network
TARGET = STiTCH
TEMPLATE = app
SOURCES += main.cpp \
    STiTCHDialog.cpp \
    ../common/iTCHServer.cpp \
    ../common/iTCHController.cpp
HEADERS += STiTCHDialog.h \
    ../common/iTCHServer.h \
    ../common/iTCHController.h
FORMS += STiTCHDialog.ui
INCLUDEPATH += ../common
