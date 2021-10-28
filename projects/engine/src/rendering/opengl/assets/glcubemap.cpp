#include "glcubemap.hpp"

#include <GL/glew.h>

// I'm dumb https://learnopengl.com/Advanced-OpenGL/Cubemaps
void GLCubemapBuffer::Populate(Cubemap *cubemap) {
    if (cubemap) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0 , GL_RGBA, cubemap->top->width, cubemap->top->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubemap->top->byteData);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}