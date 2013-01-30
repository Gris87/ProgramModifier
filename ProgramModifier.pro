#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T11:59:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProgramModifier
TEMPLATE = app

RC_FILE = Resources.rc
RESOURCES += Resources.qrc

SOURCES +=  \
            src/main.cpp \
            src/main/mainwindow.cpp \
            src/main/treedirectorydialog.cpp

HEADERS  += src/main/mainwindow.h \
            src/main/treedirectorydialog.h

FORMS    += src/main/mainwindow.ui \
            src/main/treedirectorydialog.ui
