#ifndef MANTA_GLTEXTURE_H
#define MANTA_GLTEXTURE_H

#include <assets/texture.hpp>

class GL3TextureBuffer : public TextureBuffer {
   public:
      void Populate(Texture* texture) override;

      unsigned int glTexID;
};

#endif
