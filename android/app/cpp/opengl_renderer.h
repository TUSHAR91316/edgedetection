#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility("default")))
GLuint create_gl_texture(int width, int height);

__attribute__((visibility("default")))
void update_gl_texture(GLuint texture, uint8_t* data, int width, int height);

__attribute__((visibility("default")))
void delete_gl_texture(GLuint texture);

#ifdef __cplusplus
}
#endif

#endif // OPENGL_RENDERER_H
