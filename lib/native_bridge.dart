import 'dart:ffi' as ffi;

final _nativeLib = ffi.DynamicLibrary.open("libnative_opencv.so");

// C: extern "C" int32_t create_texture();
typedef _CreateTextureNative = ffi.Int32 Function();
typedef _CreateTextureDart = int Function();

// C: extern "C" void delete_texture(int32_t texture_id);
typedef _DeleteTextureNative = ffi.Void Function(ffi.Int32);
typedef _DeleteTextureDart = void Function(int);

// C: extern "C" void process_frame(..., int32_t texture_id);
typedef _ProcessFrameNative = ffi.Void Function(
  ffi.Pointer<ffi.Uint8>,
  ffi.Int32,
  ffi.Int32,
  ffi.Int32, // textureId is int32
);
typedef _ProcessFrameDart = void Function(
  ffi.Pointer<ffi.Uint8>,
  int,
  int,
  int,
);

class NativeBridge {
  static final _createTexture =
      _nativeLib.lookupFunction<_CreateTextureNative, _CreateTextureDart>(
          'Java_com_example_edgedetection_NativeBridge_createTexture');

  static final _deleteTexture =
      _nativeLib.lookupFunction<_DeleteTextureNative, _DeleteTextureDart>(
          'Java_com_example_edgedetection_NativeBridge_deleteTexture');

  static final _processFrame =
      _nativeLib.lookupFunction<_ProcessFrameNative, _ProcessFrameDart>(
          'Java_com_example_edgedetection_NativeBridge_processFrameAndRender');

  static Future<int> createTexture() async {
    return _createTexture();
  }

  static void deleteTexture(int textureId) {
    _deleteTexture(textureId);
  }

  static void processFrame(
      ffi.Pointer<ffi.Uint8> data, int width, int height, int textureId) {
    _processFrame(data, width, height, textureId);
  }
}
