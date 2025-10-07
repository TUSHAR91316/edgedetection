import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'package:flutter/services.dart';

class CameraScreen extends StatefulWidget {
  const CameraScreen({super.key});

  @override
  State<CameraScreen> createState() => _CameraScreenState();
}

class _CameraScreenState extends State<CameraScreen> {
  static const _channel = MethodChannel('com.example.edgedetection/native');

  CameraController? _controller;
  bool _isProcessing = false;
  int? _textureId;
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    _initCamera();
  }

  Future<void> _initCamera() async {
    try {
      final cameras = await availableCameras();
      _controller = CameraController(
        cameras.first,
        ResolutionPreset.medium,
        imageFormatGroup: ImageFormatGroup.bgra8888,
        enableAudio: false,
      );
      await _controller!.initialize();

      final textureId = await _channel.invokeMethod<int>('createTexture');
      setState(() {
        _textureId = textureId;
        _isLoading = false;
      });

      await _controller!.startImageStream(_processFrame);
    } catch (e) {
      debugPrint("Camera initialization failed: $e");
      setState(() => _isLoading = false);
    }
  }

  void _processFrame(CameraImage image) async {
    if (_isProcessing || _textureId == null) return;
    _isProcessing = true;

    try {
      await _channel.invokeMethod('processFrame', {
        'bytes': image.planes[0].bytes,
        'width': image.width,
        'height': image.height,
      });
    } catch (e) {
      debugPrint('Error processing frame: $e');
    } finally {
      _isProcessing = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    if (_isLoading) {
      return const Scaffold(
        backgroundColor: Colors.black,
        body: Center(child: CircularProgressIndicator()),
      );
    }

    if (_textureId == null) {
      return const Scaffold(
        backgroundColor: Colors.black,
        body: Center(child: Text("Failed to initialize edge detection")),
      );
    }

    return Scaffold(
      backgroundColor: Colors.black,
      body: Center(
        child: Texture(textureId: _textureId!),
      ),
    );
  }

  @override
  void dispose() {
    if (_textureId != null) {
      _channel.invokeMethod('deleteTexture');
    }
    _controller?.dispose();
    super.dispose();
  }
}
