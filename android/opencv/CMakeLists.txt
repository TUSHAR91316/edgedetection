cmake_minimum_required(VERSION 3.10)
project(opencv_lib)

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/native/jni)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sdk/native/jni/include)

add_library(opencv_jni SHARED IMPORTED)
set_target_properties(opencv_jni PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/sdk/native/jni/libs/${ANDROID_ABI}/libopencv_java4.so)
