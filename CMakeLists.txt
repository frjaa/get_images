cmake_minimum_required(VERSION 3.10)
project(get_images)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flags for Debug/Release
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 11)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(realsense2 REQUIRED)

# Include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${realsense2_INCLUDE_DIRS}
)

# Add executable
add_executable(get_images get_images.cpp)

# Link libraries
target_link_libraries(get_images 
    ${OpenCV_LIBS}
    ${realsense2_LIBRARY}
)