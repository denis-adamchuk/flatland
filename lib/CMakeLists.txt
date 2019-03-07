cmake_minimum_required(VERSION 3.0)

project(flatland_lib)
set(LIB_NAME flatland_lib)

set(SOURCES
	SimpleFlatland.cpp
	SimpleFlatland.h
	SimpleMapCreator.cpp
	SimpleMapCreator.h)

add_library(${LIB_NAME} STATIC ${SOURCES})
