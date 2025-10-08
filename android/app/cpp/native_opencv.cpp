#include <jni.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <android/log.h>

#define LOG_TAG "native_opencv"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_example_edgedetection_NativeBridge_processFrameToPNG(
    JNIEnv *env, jclass, jbyteArray frameData, jint width, jint height) {
    jbyte *data = env->GetByteArrayElements(frameData, nullptr);
    if (data == nullptr) {
        LOGE("Frame data is null");
        return nullptr;
    }

    cv::Mat bgra_input(height, width, CV_8UC4, (unsigned char *)data);
    cv::Mat gray, edges;
    cv::cvtColor(bgra_input, gray, cv::COLOR_BGRA2GRAY);
    env->ReleaseByteArrayElements(frameData, data, JNI_ABORT);

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::Canny(gray, edges, 100, 200);

    // Create an opaque black image.
    cv::Mat black_background(bgra_input.size(), bgra_input.type(), cv::Scalar(0, 0, 0, 255));

    // Set the pixels to green where edges are detected.
    black_background.setTo(cv::Scalar(0, 255, 0, 255), edges);

    std::vector<uchar> png_buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    params.push_back(3);
    cv::imencode(".png", black_background, png_buffer, params);

    jbyteArray result = env->NewByteArray(png_buffer.size());
    if (result == nullptr) {
        LOGE("Failed to create new byte array");
        return nullptr;
    }
    env->SetByteArrayRegion(result, 0, png_buffer.size(), (jbyte *)png_buffer.data());

    return result;
}
