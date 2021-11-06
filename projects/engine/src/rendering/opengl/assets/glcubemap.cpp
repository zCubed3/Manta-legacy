#include "glcubemap.hpp"

#include <GL/glew.h>

// I'm dumb https://learnopengl.com/Advanced-OpenGL/Cubemaps
void GLCubemapBuffer::Populate(Cubemap *cubemap) {
    if (cubemap) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, cubemap->top->width, cubemap->top->height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, cubemap->top->byteData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, cubemap->bottom->width, cubemap->bottom->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, cubemap->bottom->byteData);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, cubemap->right->width, cubemap->right->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, cubemap->right->byteData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, cubemap->left->width, cubemap->left->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, cubemap->left->byteData);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, cubemap->forward->width, cubemap->forward->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, cubemap->forward->byteData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, cubemap->back->width, cubemap->back->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, cubemap->back->byteData);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}