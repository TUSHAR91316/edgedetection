#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility("default")))
int create_gl_texture(int width, int height);

__attribute__((visibility("default")))
void update_gl_texture(int texId, uint8_t* data, int width, int height);

__attribute__((visibility("default")))
void delete_gl_texture(int texId);

#ifdef __cplusplus
}
#endif

#endif // OPENGL_RENDERER_H
