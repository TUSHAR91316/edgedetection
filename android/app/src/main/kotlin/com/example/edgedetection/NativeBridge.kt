package com.example.edgedetection

import android.view.Surface
import io.flutter.view.TextureRegistry

class NativeBridge(private val textureRegistry: TextureRegistry) {

    private var textureEntry: TextureRegistry.SurfaceTextureEntry? = null
    var surface: Surface? = null
        private set

    /**
     * Create a new texture and return its ID to Flutter.
     */
    fun createTexture(): Long {
        val entry = textureRegistry.createSurfaceTexture()
        textureEntry = entry

        val surfaceTexture = entry.surfaceTexture()
        surfaceTexture.setDefaultBufferSize(640, 480) // Default buffer size
        surface = Surface(surfaceTexture)

        return entry.id()
    }

    /**
     * Release the texture and surface.
     */
    fun release() {
        surface?.release()
        surface = null
        textureEntry?.release()
        textureEntry = null
    }

    /**
     * Render a frame from native/OpenCV code.
     */
    fun renderFrame(frameData: ByteArray, width: Int, height: Int) {
        surface?.let {
            processFrameAndRender(frameData, width, height, it)
        }
    }

    /**
     * Native function to process frame and render to Surface.
     */
    private external fun processFrameAndRender(
        frameData: ByteArray,
        width: Int,
        height: Int,
        surface: Surface
    )

    companion object {
        init {
            System.loadLibrary("native_opencv") // Make sure your native lib matches
        }
    }
}
