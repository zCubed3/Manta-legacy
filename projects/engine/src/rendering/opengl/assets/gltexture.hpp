#ifndef MANTA_GLTEXTURE_H
#define MANTA_GLTEXTURE_H

#include <assets/texture.hpp>

class GLTextureBuffer : public TextureBuffer {
public:
    virtual ~GLTextureBuffer() override;

    void Populate(Texture *texture) override;
};

#endif
