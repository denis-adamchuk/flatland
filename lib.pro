# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = lib

QT       -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

HEADERS = \
   $$PWD/lib/SimpleFlatland.h \
   $$PWD/lib/SimpleMapCreator.h \
    lib/AdvancedMapCreator.h \
    lib/AdvancedFlatland.h \
    lib/Common.h \
    lib/CycleFinder.h \
    lib/Statistics.h

SOURCES = \
   $$PWD/lib/SimpleFlatland.cpp \
   $$PWD/lib/SimpleMapCreator.cpp \
    lib/AdvancedMapCreator.cpp \
    lib/AdvancedFlatland.cpp \
    lib/CycleFinder.cpp

INCLUDEPATH = \
    $$PWD/lib

#DEFINES = 

