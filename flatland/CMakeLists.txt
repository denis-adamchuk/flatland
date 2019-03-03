cmake_minimum_required(VERSION 3.0)

set(LIB_NAME flatland)

project(${LIB_NAME})
set(SOURCES
Flatland.cpp
Flatland.h
FlatlandCreator.cpp
FlatlandCreator.h
FlatlandCreatorItf.h)

add_library(${LIB_NAME} STATIC ${SOURCES})
