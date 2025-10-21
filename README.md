# Flutter Edge Detection App

A real-time edge detection application built with Flutter. The app uses the native Android layer to perform high-performance image processing with C++ and OpenCV, displaying a split-screen view of the original camera feed and the processed output.

---

## ✅ Features Implemented

### Android Application (Flutter)

* 📱 **Real-Time Camera Processing**: Captures the live camera stream directly within the Flutter application.
* ⚡ **Native Performance**: Edge detection is offloaded to a native C++ module using OpenCV for optimal speed, bridged to the Flutter UI via a `MethodChannel`.
* 🎨 **Custom Split-Screen Display**:
    * The top 30% of the screen shows the detected edges (green lines) on an opaque black background.
    * The bottom 70% of the screen shows the live, unprocessed camera feed.
* 🔧 **Canny Edge Detection**: The native module converts the image to grayscale, applies a Gaussian blur to reduce noise, and uses the Canny algorithm to find edges.

### Web Viewer

* **Note**: The TypeScript-based web viewer component was not implemented for this submission. The focus was on the native integration with the Flutter application.

---

## ⚙️ Setup Instructions

### Prerequisites

* **Flutter SDK**: 3.x or later
* **Android Studio**: Electric Eel or later
* **Android SDK**: API Level 24 (Android 7.0) minimum
* **Android NDK**: Version 21.3+ (for C++ compilation)
* **OpenCV Android SDK**: 4.5.0 or later

### Android Setup (for Flutter)

1.  **Install OpenCV Android SDK**

    Download the OpenCV SDK for Android and place its contents in the `android/opencv/` directory of your project. The project's `CMakeLists.txt` is pre-configured to look for it there.

2.  **Configure NDK in Android Studio**

    *   Open **Android Studio** → **Settings/Preferences** → **Appearance & Behavior** → **System Settings** → **Android SDK**.
    *   Click the **SDK Tools** tab.
    *   Check **NDK (Side by side)** and **CMake** and click **Apply** to install them.

3.  **Build and Run**

    *   **Install Dependencies**: Open a terminal in the project root and run:
        ```shell
        flutter pub get
        ```

    *   **Run the App**: Connect an Android device or start an emulator, then run:
        ```shell
        flutter run
        ```

---

##  Troubleshooting

* **Problem**: Build fails with an OpenCV or NDK error.
    * **Solution**: Ensure the OpenCV SDK is placed correctly in `android/opencv/`. Verify that the NDK is installed via the SDK Manager in Android Studio.
* **Problem**: App crashes on start with `UnsatisfiedLinkError`.
    * **Solution**: This usually means the native `.so` libraries were not built or included correctly. Run `flutter clean` and rebuild the app.
* **Problem**: Camera permission denied.
    * **Solution**: Manually grant camera permission in your device's settings: Settings → Apps → [Your App Name] → Permissions.

---

## 🧠 Architecture Overview

This project uses Flutter for the UI and delegates heavy image processing to the native Android layer to achieve real-time performance.



### Frame Flow
1.  **Camera Capture**: The Flutter UI displays a camera preview and captures a continuous stream of frames.
2.  **JNI Bridge**: Each frame is sent from Dart to the native Android platform via a `MethodChannel`.
3.  **Native Processing**: A Kotlin handler receives the camera frame and calls a native C++ function using JNI.
4.  **OpenCV Magic**: The C++ function performs the edge detection:
    *   Rotate the image to match display orientation.
    *   Convert to grayscale and apply a Gaussian blur.
    *   Use the Canny edge detector to find edges.
    *   Create a new image with the split-screen effect.
5.  **Return Value**: The final processed image is encoded as a PNG and returned as a byte array back through the `MethodChannel`.
6.  **Display**: The Flutter UI receives the byte array and displays it in an `Image.memory` widget, creating an overlay on top of the camera feed.

### Key Design Decisions
*   **Why Flutter?** To leverage its rapid UI development capabilities and cross-platform potential while still accessing native performance for critical tasks.
*   **Why MethodChannel?** It's the standard, robust way in Flutter to communicate with platform-native code, allowing us to pass frame data to C++ and receive the processed result back.
*   **Why PNG Rendering?** Sending a processed PNG back to Flutter is a straightforward way to display the result without needing a complex texture-sharing implementation between the native layer and Flutter's graphics engine. While not as performant as a direct OpenGL texture, it simplifies the architecture significantly.

---

## 📦 Project Structure
```text
edgedetection/
├── android/
│   ├── app/
│   │   ├── src/main/
│   │   │   ├── kotlin/com/example/edgedetection/
│   │   │   │   ├── MainActivity.kt      # MethodChannel handler
│   │   │   │   └── NativeBridge.kt      # JNI function declarations
│   │   │   ├── cpp/
│   │   │   │   ├── native_opencv.cpp    # C++ OpenCV implementation
│   │   │   │   └── CMakeLists.txt       # CMake build script for C++
│   └── opencv/                          # OpenCV Android SDK location
├── lib/
│   ├── screens/
│   │   └── camera_screen.dart           # Main UI screen with camera preview
│   └── native_bridge.dart               # Dart side of the native bridge
└── pubspec.yaml                         # Flutter project dependencies
```

---

## 🚀 Usage
1.  Launch the app on an Android device.
2.  Grant camera permission when prompted.
3.  The app immediately starts displaying the live camera feed with the real-time edge detection overlay. The top 30% of the screen will show the processed edges, and the bottom 70% will show the live feed.

---

## 🔧 Technologies Used
*   **Framework**: Flutter 3.x
*   **Languages**: Dart, Kotlin, C++
*   **Native Bridge**: Flutter MethodChannel (JNI)
*   **Image Processing**: OpenCV 4.5+ (C++ API)
*   **Build System**: Gradle, CMake

---

## 📝 License
This project is part of a technical assessment. Contact the repository owner for usage terms.
