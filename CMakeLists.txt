cmake_minimum_required(VERSION 3.5) # Or whatever version you use


project(gaussianPyramid)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

add_executable(gaussianPyramid main.cpp)

target_link_libraries (gaussianPyramid ${OpenCV_LIBRARIES})
