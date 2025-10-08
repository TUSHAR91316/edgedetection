package com.example.edgedetection

object NativeBridge {

    init {
        System.loadLibrary("native_opencv")
    }

    @JvmStatic
    external fun processFrameToPNG(frameData: ByteArray, width: Int, height: Int): ByteArray?
}
