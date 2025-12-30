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
  bool _showEdgeDetection = false; // Default to normal camera

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
    
    // If edge detection is disabled, clear any previous image and return
    if (!_showEdgeDetection) {
      if (_processedImage != null) {
        setState(() {
          _processedImage = null;
        });
      }
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
          // Normal Camera Preview
          CameraPreview(controller),
          
          // Edge Detection Overlay
          if (_showEdgeDetection && _processedImage != null)
            Image.memory(
              _processedImage!,
              gaplessPlayback: true,
              fit: BoxFit.cover,
            ),
            
          // Toggle Button
          Positioned(
            bottom: 30,
            left: 0,
            right: 0,
            child: Center(
              child: FloatingActionButton.extended(
                onPressed: () {
                  setState(() {
                    _showEdgeDetection = !_showEdgeDetection;
                  });
                },
                label: Text(_showEdgeDetection ? "Show Normal" : "Show Edge"),
                icon: Icon(_showEdgeDetection ? Icons.camera : Icons.grid_on),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
