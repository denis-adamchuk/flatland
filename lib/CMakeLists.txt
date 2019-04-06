cmake_minimum_required(VERSION 3.0)

set(LIB_NAME flatland_lib)

set(SOURCES
	SimpleFlatland.cpp
	SimpleFlatland.h
	SimpleMapCreator.cpp
	SimpleMapCreator.h
	AdvancedFlatland.cpp
	AdvancedFlatland.h
	AdvancedMapCreator.cpp
	AdvancedMapCreator.h
	CycleFinder.cpp
	CycleFinder.h
	Common.h
	FlatlandItf.h
	Statistics.h)

add_library(${LIB_NAME} STATIC ${SOURCES})

set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 17)
