#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T10:34:06
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

LIBSRC = ../../library/src
QLIBSRC = ../../QLibrary/src

INCLUDEPATH = $${LIBSRC}/../include $${QLIBSRC}/../include
SOURCES += main.cpp\
        form_app.cpp \
    ../../QLibrary/src/event_ctrl.cpp \
    ../../QLibrary/src/utils_qt.cpp

HEADERS  += ../../QLibrary/include/event_ctrl.h \
    form_app.h

FORMS    += main_window.ui
