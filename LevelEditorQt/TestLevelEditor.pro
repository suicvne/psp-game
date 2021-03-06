#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T20:18:28
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Rock Level Editor"

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    LIBS += -L../libs/debug -lrocklevel
    LIBS += -lopengl32
}

macx {
    ICON = icon.icns
    LIBS += -L../libs/ -lrocklevel
    QMAKE_INFO_PLIST = Info.plist
}

unix {
    LIBS += -L../libs/ -lrocklevel
}

INCLUDEPATH += ../librocklevel


SOURCES += main.cpp\
        mainwindow.cpp \
    customopenglwidget.cpp \
    gametexture.cpp \
    newleveldialog.cpp

HEADERS  += mainwindow.h \
    customopenglwidget.h \
    gametexture.h \
    newleveldialog.h

FORMS    += mainwindow.ui \
    newleveldialog.ui
