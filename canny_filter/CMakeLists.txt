cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(canny_filter_list canny_filter.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(canny_filter ${canny_filter_list})
target_link_libraries(canny_filter ${OpenCV_LIBS} )

