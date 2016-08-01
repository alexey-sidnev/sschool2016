cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(subfilter_list subfilter.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(subfilter ${subfilter_list})
target_link_libraries(subfilter ${OpenCV_LIBS} )

