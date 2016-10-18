#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T19:11:15
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CXXFLAGS += -std=c++14

TARGET = ray-tracer
TEMPLATE = app

#INCLUDEPATH += D:/dev/3rdparty/opencv-build/install/include

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
    sphere.h \
    utils.h
    #imaging.h \

#LIBS += -L"D:/dev/3rdparty/opencv-build/install/x64/vc14/lib"
#LIBS += -lopencv_core310
#LIBS += -lopencv_imgproc310
#LIBS += -lopencv_imgcodecs310
#LIBS += -lopencv_highgui310
