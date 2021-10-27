#include "gltexture.hpp"

#include <GL/glew.h>

GL3TextureBuffer::~GL3TextureBuffer() {
    if (handle != GL_INVALID_INDEX)
        glDeleteTextures(1, &handle);
}

void GL3TextureBuffer::Populate(Texture *texture) {
    if (texture) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);

        int format = GL_UNSIGNED_BYTE;

        if (texture->format == Texture::Format::Float)
            format = GL_FLOAT;

        int dataFormat = 0;

        switch (texture->dataFormat) {
            default:
                printf("Failed to populate GL3TextureBuffer, unknown Texture DataFormat, %i\n", texture->dataFormat);
                return;

            case Texture::DataFormat::RGBA:
                dataFormat = GL_RGBA;
                break;

            case Texture::DataFormat::RGB:
                dataFormat = GL_RGB;
                break;

            case Texture::DataFormat::RGBA_16F:
                dataFormat = GL_RGBA16F;
                break;
        }

        void *pixels = nullptr;

        switch (texture->format) {
            default:
                break;

            case Texture::Format::Byte:
                pixels = texture->byteData;
                break;

            case Texture::Format::Float:
                pixels = texture->floatData;
                break;
        }

        if (!texture->isDepthMap)
            glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, texture->width, texture->height, 0, GL_RGBA, format, pixels);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texture->width, texture->height, 0, GL_DEPTH_COMPONENT,
                         format, nullptr);

        int filtering = GL_NEAREST;

        if (texture->filtering == Texture::Filtering::Bilinear)
            filtering = GL_LINEAR;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    }
}
