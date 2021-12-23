#-------------------------------------------------
#
# Project created by QtCreator 2021-12-20T00:05:42
#
#-------------------------------------------------

TARGET = Parser
TEMPLATE = app

QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    Scanner.cpp \
    node.cpp \
    Parser.cpp

HEADERS  += mainwindow.h \
    Scanner.h \
    node.h \
    Parser.h \
    headers.h

FORMS    += mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
