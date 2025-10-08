import 'dart:typed_data';
import 'package:camera/camera.dart';
import 'package:flutter/material.dart';
import 'package:permission_handler/permission_handler.dart';
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
    _initPermissionAndCamera();
  }

  Future<void> _initPermissionAndCamera() async {
    final status = await Permission.camera.status;
    if (!status.isGranted) {
      final result = await Permission.camera.request();
      if (!result.isGranted) {
        if (mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(content: Text('Camera permission is required.')),
          );
        }
        return;
      }
    }
    await _initCamera();
  }

  Future<void> _initCamera() async {
    final cams = await availableCameras();
    _controller = CameraController(
      cams.first,
      ResolutionPreset.high,
      imageFormatGroup: ImageFormatGroup.bgra8888,
      enableAudio: false,
    );
    await _controller!.initialize();
    await _controller!.startImageStream(_processFrame);
    if (mounted) {
      setState(() {});
    }
  }

  void _processFrame(CameraImage image) {
    if (_isProcessing) {
      return;
    }
    _isProcessing = true;
    NativeBridge.processFrameToPNG(
      bytes: image.planes[0].bytes,
      width: image.width,
      height: image.height,
    ).then((png) {
      if (mounted && png != null) {
        setState(() {
          _processedImage = png;
        });
      }
    }).whenComplete(() {
      _isProcessing = false;
    });
  }

  @override
  void dispose() {
    _controller?.stopImageStream();
    _controller?.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final controller = _controller;
    if (controller == null || !controller.value.isInitialized) {
      return const Scaffold(body: Center(child: CircularProgressIndicator()));
    }

    return Scaffold(
      body: Stack(
        fit: StackFit.expand,
        children: [
          CameraPreview(controller),
          if (_processedImage != null)
            Image.memory(
              _processedImage!,
              gaplessPlayback: true,
              fit: BoxFit.cover,
            ),
        ],
      ),
    );
  }
}
