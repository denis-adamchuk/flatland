cmake_minimum_required(VERSION 3.0)

set(APP_NAME flatland_console)

set(SOURCES
	main_console.cpp)

add_executable(${APP_NAME} ${SOURCES})
target_link_libraries(${APP_NAME} flatland_lib)
target_include_directories(${APP_NAME}
	PRIVATE ..)
