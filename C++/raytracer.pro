#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T11:56:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = frangiray
TEMPLATE = app

INCLUDEPATH += ../../../3rdparty/jsoncpp/dist/json

SOURCES += \
    main.cpp \
    ui/mainwindow.cpp \
    ui/viewport.cpp \
    ui/colorpicker.cpp \
    ../../../3rdparty/jsoncpp/dist/jsoncpp.cpp \
    math/vector.cpp \
    math/matrix.cpp \
    node/abstractnode.cpp \
    node/camera.cpp \
    node/sphere.cpp \
    node/scene.cpp \
    tracer/tracer.cpp \
    image/buffer.cpp \
    image/pixel.cpp \
    ui/traceworker.cpp \
    node/surfaceattributes.cpp

FORMS += \
    ui/mainwindow.ui

HEADERS += \
    ui/mainwindow.h \
    ui/viewport.h \
    ui/colorpicker.h \
    json.h \
    json-forwards.h \
    common/typedefs.h \
    math/matrix.h \
    math/vector.h \
    math/ray.h \
    math/rayhit.h \
    node/abstractnode.h \
    node/camera.h \
    node/sphere.h \
    node/scene.h \
    tracer/tracer.h \
    image/color.h \
    image/buffer.h \
    image/pixel.h \
    ui/traceworker.h \
    common/constants.h \
    common/enums.h \
    node/surfaceattributes.h
