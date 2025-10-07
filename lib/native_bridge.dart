import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:ffi/ffi.dart';

typedef process_frame_native_t = ffi.Void Function(ffi.Pointer<ffi.Uint8>, ffi.Int32, ffi.Int32);
typedef process_frame_dart_t = void Function(ffi.Pointer<ffi.Uint8>, int, int);

typedef create_texture_native_t = ffi.Int32 Function(ffi.Int32, ffi.Int32);
typedef create_texture_dart_t = int Function(int, int);

typedef update_texture_native_t = ffi.Void Function(ffi.Int32, ffi.Pointer<ffi.Uint8>, ffi.Int32, ffi.Int32);
typedef update_texture_dart_t = void Function(int, ffi.Pointer<ffi.Uint8>, int, int);

typedef delete_texture_native_t = ffi.Void Function(ffi.Int32);
typedef delete_texture_dart_t = void Function(int);

// open the library
final dylib = Platform.isAndroid
    ? ffi.DynamicLibrary.open("libnative_opencv.so")
    : ffi.DynamicLibrary.process();

// lookups
final processFrame = dylib
    .lookup<ffi.NativeFunction<process_frame_native_t>>('processFrame')
    .asFunction<process_frame_dart_t>();

final createGlTexture = dylib
    .lookup<ffi.NativeFunction<create_texture_native_t>>('create_gl_texture')
    .asFunction<create_texture_dart_t>();

final updateGlTexture = dylib
    .lookup<ffi.NativeFunction<update_texture_native_t>>('update_gl_texture')
    .asFunction<update_texture_dart_t>();

final deleteGlTexture = dylib
    .lookup<ffi.NativeFunction<delete_texture_native_t>>('delete_gl_texture')
    .asFunction<delete_texture_dart_t>();
