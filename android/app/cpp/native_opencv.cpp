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

    // Create a Mat from the BGRA data and clone it to a new Mat for processing.
    cv::Mat bgra_input(height, width, CV_8UC4, (unsigned char *)data);
    cv::Mat processed_image = bgra_input.clone();

    // Release the original byte array as we have a copy.
    env->ReleaseByteArrayElements(frameData, data, JNI_ABORT);

    // Perform edge detection.
    cv::Mat gray, edges;
    cv::cvtColor(processed_image, gray, cv::COLOR_BGRA2GRAY);
    
    // Apply a Gaussian blur to reduce noise and sensitivity.
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    
    // Adjust Canny thresholds for less sensitive edge detection.
    cv::Canny(gray, edges, 100, 200);
    
    // Overlay green edges on the processed image.
    processed_image.setTo(cv::Scalar(0, 255, 0, 255), edges);

    // Encode the processed image to PNG.
    std::vector<uchar> png_buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    params.push_back(3);
    cv::imencode(".png", processed_image, png_buffer, params);

    jbyteArray result = env->NewByteArray(png_buffer.size());
    if (result == nullptr) {
        LOGE("Failed to create new byte array");
        return nullptr;
    }
    env->SetByteArrayRegion(result, 0, png_buffer.size(), (jbyte *)png_buffer.data());

    return result;
}
