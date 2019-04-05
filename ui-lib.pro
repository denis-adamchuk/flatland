TARGET = ui-lib

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

SOURCES += \
        $$PWD/ui-lib/AdjustableTimer.cpp \
        $$PWD/ui-lib/RenderAreaSimple.cpp \
        $$PWD/ui-lib/RenderAreaAdvanced.cpp \
        $$PWD/ui-lib/RenderAreaBase.cpp

HEADERS += \
        $$PWD/ui-lib/RenderAreaBase.h \
        $$PWD/ui-lib/AdjustableTimer.h \
        $$PWD/ui-lib/RenderAreaSimple.h \
        $$PWD/ui-lib/RenderAreaAdvanced.h \
        $$PWD/ui-lib/RenderAreaFactory.h

INCLUDEPATH = \
    $$PWD/lib
