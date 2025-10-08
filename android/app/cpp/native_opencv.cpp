#include <jni.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <android/log.h>

#define LOG_TAG "native_opencv"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jbyteArray JNICALL
com_example_edgedetection_MainActivity_processFrameToPNG(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray frameData,
        jint width,
        jint height) {

    jbyte* data = env->GetByteArrayElements(frameData, nullptr);
    if (data == nullptr) {
        LOGE("Frame data is null");
        return nullptr;
    }

    // Assuming input is BGRA format from Flutter camera plugin
    cv::Mat bgra(height, width, CV_8UC4, (unsigned char*)data);
    cv::Mat gray, edges;

    // Process the image
    cv::cvtColor(bgra, gray, cv::COLOR_BGRA2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);
    cv::Canny(gray, edges, 80, 180);
    bgra.setTo(cv::Scalar(0, 255, 0, 255), edges); // Draw green edges on original image

    env->ReleaseByteArrayElements(frameData, data, JNI_ABORT);

    // Encode the processed image to PNG
    std::vector<uchar> png_buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    params.push_back(3); // A good compromise for speed vs size
    cv::imencode(".png", bgra, png_buffer, params);

    jbyteArray result = env->NewByteArray(png_buffer.size());
    if (result == nullptr) {
        LOGE("Failed to create new byte array");
        return nullptr; // out of memory error
    }
    env->SetByteArrayRegion(result, 0, png_buffer.size(), (jbyte*)png_buffer.data());

    LOGI("PNG processing complete, size: %zu", png_buffer.size());

    return result;
}
