#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T19:11:15
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ray-tracer
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    buffer.cpp \
    qtutils.cpp \
    renderer.cpp

HEADERS  += mainwindow.h \
    buffer.h \
    qtutils.h \
    renderer.h \
    ray.h \
    matrix.h \
    vector.h \
    camera.h \
    scene.h \
    traceable.h \
    color.h \
    sphere.h