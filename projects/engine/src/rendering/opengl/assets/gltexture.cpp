#include "gltexture.hpp"

#include <GL/glew.h>

void GL3TextureBuffer::Populate(Texture* texture) {
   if (texture) {
      glGenTextures(1, &glTexID);
      glBindTexture(GL_TEXTURE_2D, glTexID);

      int format = GL_UNSIGNED_BYTE;

      if (texture->format == Texture::Format::Float)
	 format = GL_FLOAT;

      int dataFormat = GL_RGBA;

      if (texture->dataFormat == Texture::DataFormat::RGBA_16F)
	 dataFormat = GL_RGBA16F;

      if (!texture->isDepthMap)
	 glTexImage2D(GL_TEXTURE_2D,  0, dataFormat, texture->width, texture->height, 0, GL_RGBA, format, nullptr);
      else
	 glTexImage2D(GL_TEXTURE_2D,  0, GL_DEPTH_COMPONENT, texture->width, texture->height, 0, GL_DEPTH_COMPONENT, format, nullptr);

      int filtering = GL_NEAREST;

      if (texture->filtering == Texture::Filtering::Bilinear)
	 filtering = GL_LINEAR;

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
   }
}
