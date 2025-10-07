import 'dart:ffi' as ffi;
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'package:ffi/ffi.dart';
import '../native_bridge.dart';

class CameraScreen extends StatefulWidget {
  const CameraScreen({super.key});

  @override
  State<CameraScreen> createState() => _CameraScreenState();
}

class _CameraScreenState extends State<CameraScreen> {
  CameraController? _controller;
  Uint8List? _processedImage;
  bool _isProcessing = false;

  @override
  void initState() {
    super.initState();
    _initCamera();
  }

  Future<void> _initCamera() async {
    final cameras = await availableCameras();
    _controller = CameraController(
      cameras.first,
      ResolutionPreset.medium,
      imageFormatGroup: ImageFormatGroup.bgra8888,
    );
    await _controller!.initialize();
    await _controller!.startImageStream(_processFrame);
    setState(() {});
  }

  void _processFrame(CameraImage image) async {
    if (_isProcessing) return;
    _isProcessing = true;

    try {
      final bytes = image.planes[0].bytes;
      final width = image.width;
      final height = image.height;

      final ptr = malloc.allocate<ffi.Uint8>(bytes.length);
      ptr.asTypedList(bytes.length).setAll(0, bytes);

      processFrame(ptr, width, height);

      final result = Uint8List.fromList(ptr.asTypedList(bytes.length));
      malloc.free(ptr);

      setState(() => _processedImage = result);
    } catch (e) {
      debugPrint('Error processing frame: $e');
    } finally {
      _isProcessing = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    if (_controller == null || !_controller!.value.isInitialized) {
      return const Center(child: CircularProgressIndicator());
    }

    return Scaffold(
      backgroundColor: Colors.black,
      body: Row(
        children: [
          Expanded(child: CameraPreview(_controller!)), // left
          if (_processedImage != null)
            Expanded(
              child: Image.memory(
                _processedImage!,
                gaplessPlayback: true,
                fit: BoxFit.cover,
              ),
            ),
        ],
      ),
    );
  }
}
