cmake_minimum_required(VERSION 3.10.2)
project(native_opencv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Native sources
add_library(native_opencv SHARED
        cpp/native_opencv.cpp
        cpp/opencv_processor.cpp
)

# Includes (if you have headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)

# Point to your OpenCV Android SDK
set(OpenCV_DIR "F:/Project/edgedetection/android/opencv/sdk/OpenCV-android-sdk/sdk/native/jni")

# Find OpenCV with modules including imgcodecs for imencode
find_package(OpenCV REQUIRED core imgproc imgcodecs)

if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_DIR}")
    include_directories(${OpenCV_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "OpenCV not found. Please set OpenCV_DIR correctly.")
endif()

# Android libs
find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)

# Link libraries
target_link_libraries(
        native_opencv
        ${OpenCV_LIBS}
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
)
