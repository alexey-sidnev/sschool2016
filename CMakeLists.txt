cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(sschool2016 CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_custom_target(copy-test-data
                  COMMAND "${CMAKE_COMMAND}" -E copy_directory
                                "${CMAKE_CURRENT_LIST_DIR}/data"
                                "${CMAKE_BINARY_DIR}/data"
                  VERBATIM)

function(add_example example_name)
  add_subdirectory(${example_name})
  add_dependencies(${example_name} copy-test-data)
endfunction(add_example)

add_example(image_processing)
add_example(video_playback)
add_example(canny_filter)
add_example(subfilter)
add_example(floating_filter)
add_example(mouse_callback)
add_example(convex_hull)

