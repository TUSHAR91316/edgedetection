import 'dart:ffi' as ffi;
import 'dart:io';

typedef ProcessFrameNative = ffi.Void Function(ffi.Pointer<ffi.Uint8>, ffi.Int32, ffi.Int32);
typedef ProcessFrame = void Function(ffi.Pointer<ffi.Uint8>, int, int);

final dylib = Platform.isAndroid
    ? ffi.DynamicLibrary.open('libnative_opencv.so')
    : ffi.DynamicLibrary.process();

final ProcessFrame processFrame = dylib
    .lookup<ffi.NativeFunction<ProcessFrameNative>>('processFrame')
    .asFunction();
