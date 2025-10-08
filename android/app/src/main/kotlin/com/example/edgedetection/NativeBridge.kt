package com.example.edgedetection

object NativeBridge {
    init {
        System.loadLibrary("native_opencv")
    }

    // JNI binding - this matches the C++ JNI name: Java_com_example_edgedetection_NativeBridge_processFrameToPNG
    external fun processFrameToPNG(frameData: ByteArray, width: Int, height: Int): ByteArray?
}
