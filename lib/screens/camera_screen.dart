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
  CameraController? controller;
  Uint8List? processedImage;
  static const platform = MethodChannel("edge_detection_channel");

  @override
  void initState() {
    super.initState();
    initCamera();
  }

  Future<void> initCamera() async {
    final cameras = await availableCameras();
    controller = CameraController(cameras.first, ResolutionPreset.medium);
    await controller!.initialize();

    controller!.startImageStream((CameraImage image) async {
      final bytes = image.planes[0].bytes;
      final result = await platform.invokeMethod("processFrame", {
        "frameData": bytes,
        "width": image.width,
        "height": image.height,
      });
      setState(() => processedImage = Uint8List.fromList(List<int>.from(result)));
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: processedImage == null
          ? const Center(child: CircularProgressIndicator())
          : Image.memory(processedImage!),
    );
  }
}
