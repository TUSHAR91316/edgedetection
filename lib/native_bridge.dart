import 'dart:typed_data';

import 'package:flutter/services.dart';

class NativeBridge {
  static const MethodChannel _channel = MethodChannel('edge_detector_channel');

  static Future<Uint8List?> processFrameToPNG({
    required Uint8List bytes,
    required int width,
    required int height,
  }) async {
    try {
      final Uint8List? result = await _channel.invokeMethod('processFrame', {
        'bytes': bytes,
        'width': width,
        'height': height,
      });
      return result;
    } on PlatformException catch (e) {
      print("Failed to process frame: '${e.message}'.");
      return null;
    }
  }
}
