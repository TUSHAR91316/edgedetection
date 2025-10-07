#include "opengl_renderer.h"
#include <GLES2/gl2.h>
#include <android/log.h>

#define LOG_TAG "opengl_renderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

int create_gl_texture(int width, int height) {
    if (width <= 0 || height <= 0) {
        LOGE("create_gl_texture: invalid size %dx%d", width, height);
        return 0;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    if (tex == 0) {
        LOGE("create_gl_texture: glGenTextures returned 0");
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    LOGI("create_gl_texture: created tex %u (%dx%d)", tex, width, height);
    return (int)tex;
}

void update_gl_texture(int texId, uint8_t* data, int width, int height) {
    if (texId == 0 || data == nullptr || width <= 0 || height <= 0) {
        LOGE("update_gl_texture: invalid args texId=%d data=%p %dx%d", texId, data, width, height);
        return;
    }

    GLuint tex = (GLuint)texId;
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void delete_gl_texture(int texId) {
    if (texId == 0) return;
    GLuint tex = (GLuint)texId;
    glDeleteTextures(1, &tex);
    LOGI("delete_gl_texture: deleted tex %u", tex);
}
