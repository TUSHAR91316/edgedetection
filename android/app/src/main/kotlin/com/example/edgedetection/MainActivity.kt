package com.example.edgedetection

import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel

class MainActivity : FlutterActivity() {

    private lateinit var nativeBridge: NativeBridge
    private val channelName = "com.example.edgedetection/native"

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)

        // Pass FlutterEngine's textureRegistry to NativeBridge
        nativeBridge = NativeBridge(flutterEngine.renderer) // or flutterEngine.textureRegistry

        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, channelName)
            .setMethodCallHandler { call, result ->
                when (call.method) {
                    "createTexture" -> {
                        result.success(nativeBridge.createTexture())
                    }
                    "deleteTexture" -> {
                        nativeBridge.release()
                        result.success(null)
                    }
                    "processFrame" -> {
                        val args = call.arguments as Map<String, Any>
                        val frameData = args["bytes"] as ByteArray
                        val width = args["width"] as Int
                        val height = args["height"] as Int
                        nativeBridge.renderFrame(frameData, width, height)
                        result.success(null)
                    }
                    else -> result.notImplemented()
                }
            }
    }

    override fun cleanUpFlutterEngine(flutterEngine: FlutterEngine) {
        super.cleanUpFlutterEngine(flutterEngine)
        nativeBridge.release()
    }
}