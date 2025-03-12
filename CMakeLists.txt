cmake_minimum_required(VERSION 3.10)
project(get_images)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(get_images get_images.cpp)
target_link_libraries(get_images ${OpenCV_LIBS})