#include "opencv_processor.h"
#include <opencv2/opencv.hpp>
#include <android/log.h>

#define LOG_TAG "opencv_processor"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// The definition of processFrame is now in native_opencv.cpp.
// This file is kept for future OpenCV processing functions.
