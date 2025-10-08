![Screenshot_2025-10-08-13-39-45-68_39bef26cf97d0f1b16372f0891a026f7](https://github.com/user-attachments/assets/6666fc4b-1b23-4461-9fbf-e582f6ab350e)# Edge Detection App

This project is an Android application that performs real-time edge detection on a live camera feed using a native C++/OpenCV backend. The UI is built with Flutter.

## Features

*   **Real-Time Camera Processing:** The application captures the live camera feed for processing.
*   **Native Edge Detection:** Camera frames are passed from the application layer to a native C++ module using Flutter's MethodChannel. The C++ layer uses the OpenCV library to perform Canny Edge Detection.
*   **Split-Screen Display:** The processed output is displayed in a split-screen format:
    *   The top 30% of the screen shows the detected edges (green lines on an opaque black background).
    *   The bottom 70% shows the original, unprocessed camera feed.

## Architecture and Frame Flow

The application is built using Flutter for the UI and a native C++ backend for image processing.

1.  The Flutter UI, running in `camera_screen.dart`, displays a full-screen camera preview.
2.  A stream of camera frames is sent from Dart to the native Android platform via a `MethodChannel` defined in `native_bridge.dart`.
3.  The Kotlin `MainActivity.kt` receives the frame data and passes it to the native C++ function.
4.  The C++ function in `native_opencv.cpp` performs the image processing using OpenCV:
    *   The image is rotated to match the device's display orientation.
    *   It's converted to grayscale.
    *   A Gaussian blur is applied to reduce noise.
    *   The Canny edge detector algorithm is used to find edges.
    *   A new image is composed, showing the edges in the top portion and the original feed in the bottom.
5.  This final image is encoded as a PNG and sent back to Flutter, where it is displayed over the camera feed.

## Tech Stack

*   **UI Framework:** Flutter / Dart
*   **Android Host:** Kotlin
*   **Native Bridge:** Flutter MethodChannel
*   **Image Processing:** C++ with OpenCV
*   **Build System:** CMake for C++

## Setup and Installation

### Prerequisites

*   Flutter SDK
*   Android Studio
*   Android NDK
*   OpenCV for Android SDK

### Instructions

1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/TUSHAR91316/edgedetection.git
    ```
2.  **Download OpenCV for Android:** Download the OpenCV Android SDK and place its contents inside the `android/opencv/` directory in the project root.
3.  **Install Dependencies:**
    ```bash
    flutter pub get
    ```
4.  **Run the App:** Connect an Android device and run the app.
    ```bash
    flutter run
    ```
