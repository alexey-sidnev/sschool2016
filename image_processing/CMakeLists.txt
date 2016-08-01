cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(image_processing_list improc.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(image_processing ${image_processing_list})
target_link_libraries(image_processing ${OpenCV_LIBS} )

