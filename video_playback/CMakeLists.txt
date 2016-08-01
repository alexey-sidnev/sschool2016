cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(OpenCV REQUIRED)

set(video_playback_list video_playback.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(video_playback ${video_playback_list})
target_link_libraries(video_playback ${OpenCV_LIBS} )

