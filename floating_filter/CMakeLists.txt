cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(floating_filter_list floating_filter.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(floating_filter ${floating_filter_list})
target_link_libraries(floating_filter ${OpenCV_LIBS} )

