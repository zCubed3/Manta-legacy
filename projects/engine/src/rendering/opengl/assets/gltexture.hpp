#ifndef MANTA_GLTEXTURE_H
#define MANTA_GLTEXTURE_H

#include <assets/texture.hpp>

class GL3TextureBuffer : public TextureBuffer {
public:
    virtual ~GL3TextureBuffer() override;

    void Populate(Texture *texture) override;
};

#endif
