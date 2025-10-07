#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>

#define LOG_TAG "native_opencv"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

__attribute__((visibility("default")))
void processFrame(uint8_t* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        LOGE("Invalid args to processFrame");
        return;
    }

    // Flutter camera gives BGRA format, not RGBA
    cv::Mat bgra(height, width, CV_8UC4, data);
    cv::Mat gray, edges;

    // Convert to grayscale
    cv::cvtColor(bgra, gray, cv::COLOR_BGRA2GRAY);

    // Apply Gaussian blur for smoother edges
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);

    // Detect edges using Canny
    cv::Canny(gray, edges, 80, 180);

    // Use the edge mask to set the color of the detected edges to green
    bgra.setTo(cv::Scalar(0, 255, 0, 255), edges);
}

}
