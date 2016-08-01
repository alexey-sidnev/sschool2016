cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(mouse_callback_list mouse_callback.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(mouse_callback ${mouse_callback_list})
target_link_libraries(mouse_callback ${OpenCV_LIBS} )

