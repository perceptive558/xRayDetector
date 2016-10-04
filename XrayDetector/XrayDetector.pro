#-------------------------------------------------
#
# Project created by QtCreator 2016-08-15T15:06:24
#
#-------------------------------------------------

QT       += sql core gui serialport printsupport qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XrayDetector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    seriallink.cpp \
    mymessage.cpp


HEADERS  += mainwindow.h \
    seriallink.h \
    connection.h \
    mymessage.h


FORMS    += mainwindow.ui \
    mymessage.ui

RESOURCES += \
    res.qrc

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}
CONFIG += "lang-zh_CN"
CONFIG += "handwriting"


#includes
INCLUDEPATH += "$$(HALCONROOT)/include"
INCLUDEPATH += "$$(HALCONROOT)/include/cpp"
INCLUDEPATH += "./inc"

#libs
QMAKE_LIBDIR    += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
unix:LIBS   += -lhalconcpp -lhalcon -ldl -lpthread
win32:LIBS  += halconcpp.lib halcon.lib
