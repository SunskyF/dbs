#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T09:50:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = library
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    book.cpp \
    logindlg.cpp

HEADERS  += mainwindow.h \
    book.h \
    logindlg.h \
    db_utl.h \
    seach.h

FORMS    += mainwindow.ui \
    logindlg.ui

DISTFILES +=
