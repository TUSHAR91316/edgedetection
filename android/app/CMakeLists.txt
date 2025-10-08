cmake_minimum_required(VERSION 3.10.2)

project(native_opencv)

# ✅ Tell CMake to use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Define your native source files
add_library(
        native_opencv
        SHARED
        cpp/native_opencv.cpp
        cpp/opencv_processor.cpp
)

# ✅ Include JNI headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ✅ Link OpenCV
# Change this path to where you installed OpenCV for Android!
set(OpenCV_DIR "F:/Project/edgedetection/android/opencv/sdk/OpenCV-android-sdk/sdk/native/jni")  # ✅ Replace this path if different
find_package(OpenCV REQUIRED core imgproc imgcodecs)

if(OpenCV_FOUND)
    message(STATUS "✅ OpenCV found: ${OpenCV_INCLUDE_DIRS}")
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(native_opencv ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "❌ OpenCV not found! Please set OpenCV_DIR correctly.")
endif()

# ✅ Link standard Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)

# ✅ Link everything together
target_link_libraries(
        native_opencv
        ${OpenCV_LIBS}
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
)
