#-------------------------------------------------
#
# Project created by QtCreator 2017-06-13T19:20:44
#
#-------------------------------------------------

QT       -= core gui

TARGET = rocklevel
TEMPLATE = lib

DEFINES += LIBROCKLEVEL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    rocklevel/serialization_reader.c \
    rocklevel/serializer.c \
    rocklevel/tile.c \
    rocklevel/tilemap.c \
    rocklevel/vector.c \
    rocklevel/camera.c

HEADERS +=\
    rocklevel/serialization_reader.h \
    rocklevel/serializer.h \
    rocklevel/tile.h \
    rocklevel/tilemap.h \
    rocklevel/tilemap_serializer.h \
    rocklevel/vector.h \
    rocklevel/camera.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
