cmake_minimum_required(VERSION 3.0)

set(APP_NAME console2)

set(SOURCES
	main_console2.cpp)

add_executable(${APP_NAME} ${SOURCES})
target_link_libraries(${APP_NAME} flatland_lib)
target_include_directories(${APP_NAME}
	PRIVATE ..)
set_target_properties(${APP_NAME} PROPERTIES CXX_STANDARD 17)
