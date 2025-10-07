cmake_minimum_required(VERSION 3.10.2)
project(native_opencv)

add_library(native_opencv SHARED native_opencv.cpp opencv_processor.cpp opengl_renderer.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_library(log-lib log)
target_link_libraries(native_opencv ${OpenCV_LIBS} ${log-lib} GLESv2)
