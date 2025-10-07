#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#define LOG_TAG "native_opencv"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT void JNICALL
Java_com_example_edgedetection_MainActivity_processFrameAndRender(
        JNIEnv* env, jobject /* this */, jbyteArray frameData, jint width, jint height, jobject surface) {

    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("ANativeWindow_fromSurface failed");
        return;
    }

    ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer buffer;
    if (ANativeWindow_lock(window, &buffer, nullptr) < 0) {
        LOGE("ANativeWindow_lock failed");
        ANativeWindow_release(window);
        return;
    }

    jbyte* data = env->GetByteArrayElements(frameData, nullptr);
    if (data == nullptr) {
        LOGE("Frame data is null");
        ANativeWindow_unlockAndPost(window);
        ANativeWindow_release(window);
        return;
    }

    cv::Mat bgra(height, width, CV_8UC4, (unsigned char*)data);
    cv::Mat gray, edges;

    cv::cvtColor(bgra, gray, cv::COLOR_BGRA2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);
    cv::Canny(gray, edges, 80, 180);
    bgra.setTo(cv::Scalar(0, 255, 0, 255), edges);

    // Copy the processed frame to the window buffer
    uint8_t* dst = static_cast<uint8_t*>(buffer.bits);
    int dstStride = buffer.stride * 4; // stride is in pixels
    int srcStride = width * 4;
    for (int y = 0; y < height; y++) {
        memcpy(dst + y * dstStride, bgra.data + y * srcStride, srcStride);
    }

    ANativeWindow_unlockAndPost(window);
    ANativeWindow_release(window);
    env->ReleaseByteArrayElements(frameData, data, JNI_ABORT);
}
