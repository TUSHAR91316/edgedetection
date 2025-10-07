#include "opencv_processor.h"
#include <opencv2/opencv.hpp>
#include <android/log.h>
#include <cstring>

#define LOG_TAG "opencv_processor"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

void processFrame(uint8_t* data, int width, int height) {
    if (data == nullptr || width <= 0 || height <= 0) {
        LOGE("processFrame: invalid args (data=%p,w=%d,h=%d)", data, width, height);
        return;
    }

    cv::Mat rgba(height, width, CV_8UC4, data);

    try {
        cv::Mat gray;
        cv::cvtColor(rgba, gray, cv::COLOR_RGBA2GRAY);

        cv::Mat blurred;
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 0);

        cv::Mat edges;
        const double thresh1 = 80.0;
        const double thresh2 = 180.0;
        cv::Canny(blurred, edges, thresh1, thresh2);

        cv::Mat edgesRGBA;
        cv::cvtColor(edges, edgesRGBA, cv::COLOR_GRAY2RGBA);

        if (edgesRGBA.total() == (size_t)width * (size_t)height) {
            std::memcpy(rgba.data, edgesRGBA.data, (size_t)width * (size_t)height * 4);
        } else {
            LOGE("processFrame: size mismatch when copying back results");
        }
    } catch (const cv::Exception& e) {
        LOGE("OpenCV exception: %s", e.what());
    } catch (const std::exception& e) {
        LOGE("std::exception: %s", e.what());
    } catch (...) {
        LOGE("Unknown exception in processFrame");
    }
}
