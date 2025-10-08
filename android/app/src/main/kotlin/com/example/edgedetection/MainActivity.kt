package com.example.edgedetection

import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel

class MainActivity : FlutterActivity() {
    private val CHANNEL = "edge_detection_channel"

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)

        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, CHANNEL)
            .setMethodCallHandler { call, result ->
                if (call.method == "processFrame") {
                    try {
                        val frameData = call.argument<ByteArray>("frameData")
                        val width = call.argument<Int>("width") ?: 0
                        val height = call.argument<Int>("height") ?: 0

                        if (frameData != null && width > 0 && height > 0) {
                            val processed = NativeBridge.processFrameToPNG(frameData, width, height)
                            result.success(processed)
                        } else {
                            result.error("INVALID_INPUT", "Invalid frame data", null)
                        }
                    } catch (e: Exception) {
                        result.error("NATIVE_ERROR", e.localizedMessage, null)
                    }
                } else {
                    result.notImplemented()
                }
            }
    }
}
