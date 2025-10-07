cmake_minimum_required(VERSION 3.10.2)
project("native_opencv")

# Set OpenCV path. Using forward slashes is crucial for CMake on Windows.
# This should point to the directory containing OpenCVConfig.cmake.
set(OpenCV_DIR "F:/Project/edgedetection/android/opencv/sdk/OpenCV-android-sdk/sdk/native/jni")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all your C++ source files here.
add_library(native_opencv SHARED native_opencv.cpp opencv_processor.cpp opengl_renderer.cpp)

find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)
find_library(EGL-lib EGL)

target_link_libraries(
        native_opencv
        ${OpenCV_LIBS}
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
        ${EGL-lib})

