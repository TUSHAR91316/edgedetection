#ifndef OPENCV_PROCESSOR_H
#define OPENCV_PROCESSOR_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility("default")))
void processFrame(uint8_t* data, int width, int height);

#ifdef __cplusplus
}
#endif

#endif // OPENCV_PROCESSOR_H
