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

CONFIG (debug, debug|release) {
    DESTDIR = debug/
    OBJECTS_DIR = debug/gen
    MOC_DIR = debug/gen
    RCC_DIR = debug/gen
} else {
    DESTDIR = release/
    OBJECTS_DIR = release/gen
    MOC_DIR = release/gen
    RCC_DIR = release/gen
}



SOURCES +=  \
            src/main.cpp \
            src/main/mainwindow.cpp \
            src/main/treedirectorydialog.cpp \
            src/main/filetreewidgetitem.cpp

HEADERS  += src/main/mainwindow.h \
            src/main/treedirectorydialog.h \
            src/main/filetreewidgetitem.h

FORMS    += src/main/mainwindow.ui \
            src/main/treedirectorydialog.ui
