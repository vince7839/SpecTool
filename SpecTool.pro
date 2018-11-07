#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T10:49:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpecTool
TEMPLATE = app


SOURCES +=\
    test/cmdtest.cpp \
    test/defaulttest.cpp \
    test/equaltest.cpp \
    test/featuretest.cpp \
    test/intenttest.cpp \
    test/packagetest.cpp \
    test/proptest.cpp \
    util/devicelistener.cpp \
    util/executor.cpp \
    util/specbuilder.cpp \
    util/spectestthread.cpp \
    util/specutil.cpp \
    widget/buildspecwidget.cpp \
    widget/mainwindow.cpp \
    test/sdktest.cpp \
    main.cpp \
    widget/resultwidget.cpp \
    widget/sdkwidget.cpp \
    util/system.cpp

HEADERS  += \
    test/cmdtest.h \
    test/defaulttest.h \
    test/equaltest.h \
    test/featuretest.h \
    test/intenttest.h \
    test/packagetest.h \
    test/proptest.h \
    test/spectest.h \
    util/devicelistener.h \
    util/executor.h \
    util/specbuilder.h \
    util/spectestthread.h \
    util/specutil.h \
    widget/buildspecwidget.h \
    widget/mainwindow.h \
    test/sdktest.h \
    widget/resultwidget.h \
    widget/sdkwidget.h \
    util/system.h

FORMS    += \
    widget/buildspecwidget.ui \
    widget/mainwindow.ui \
    widget/resultwidget.ui \
    widget/sdkwidget.ui
