cmake_minimum_required(VERSION 3.0)

set(APP_NAME flatland_ut)

project(${APP_NAME})
set(SOURCES
main.cpp
Test_Flatland.cpp)

# TODO Add headers from dependent projects here

add_executable(${APP_NAME} ${SOURCES})
target_link_libraries(${APP_NAME} flatland)
