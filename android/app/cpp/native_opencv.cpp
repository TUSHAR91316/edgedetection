// The implementation has been split into:
// - opengl_renderer.cpp
// - opencv_processor.cpp

#include "opengl_renderer.h"
#include "opencv_processor.h"
#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>
#include <cstring>

#define LOG_TAG "native_opencv"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

// Simple RGBA frame processing (Canny Edge Detection)
__attribute__((visibility("default")))
void processFrame(uint8_t* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        LOGE("Invalid args passed to processFrame()");
        return;
    }

    cv::Mat rgba(height, width, CV_8UC4, data);
    cv::Mat gray, edges, edgesRGBA;

    cv::cvtColor(rgba, gray, cv::COLOR_RGBA2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);
    cv::Canny(gray, edges, 80, 180);
    cv::cvtColor(edges, edgesRGBA, cv::COLOR_GRAY2RGBA);

    std::memcpy(rgba.data, edgesRGBA.data, width * height * 4);
}

}
