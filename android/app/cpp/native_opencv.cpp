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

    // Create a Mat from the camera data
    cv::Mat bgra_input(height, width, CV_8UC4, (unsigned char *)data);
    env->ReleaseByteArrayElements(frameData, data, JNI_ABORT);

    // Rotate the image 90 degrees clockwise to match the display orientation.
    cv::Mat rotated_image;
    cv::rotate(bgra_input, rotated_image, cv::ROTATE_90_CLOCKWISE);

    // Clone the rotated image to create the final output image.
    cv::Mat output_image = rotated_image.clone();

    // --- Edge Detection on the rotated image ---
    cv::Mat gray, edges;
    cv::cvtColor(rotated_image, gray, cv::COLOR_BGRA2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::Canny(gray, edges, 100, 200);

    // --- Create a Region of Interest (ROI) for the top 30% of the rotated image ---
    int roi_height = rotated_image.rows * 0.3;
    cv::Rect top_roi_rect(0, 0, rotated_image.cols, roi_height);

    cv::Mat output_roi = output_image(top_roi_rect);
    cv::Mat edges_roi = edges(top_roi_rect);

    // Set the background of the top ROI to opaque black.
    output_roi.setTo(cv::Scalar(0, 0, 0, 255));

    // Draw the green edges onto the black background in the top ROI.
    output_roi.setTo(cv::Scalar(0, 255, 0, 255), edges_roi);

    // --- Encode the final modified image to PNG ---
    std::vector<uchar> png_buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    params.push_back(3);
    cv::imencode(".png", output_image, png_buffer, params);

    jbyteArray result = env->NewByteArray(png_buffer.size());
    if (result == nullptr) {
        LOGE("Failed to create new byte array");
        return nullptr;
    }
    env->SetByteArrayRegion(result, 0, png_buffer.size(), (jbyte *)png_buffer.data());

    return result;
}
