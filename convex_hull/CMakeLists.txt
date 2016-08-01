cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(convex_hull_list convex_hull.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(convex_hull ${convex_hull_list})
target_link_libraries(convex_hull ${OpenCV_LIBS} )

