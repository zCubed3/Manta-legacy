#ifndef MANTA_GLCUBEMAP_HPP
#define MANTA_GLCUBEMAP_HPP

#include <assets/cubemap.hpp>

class GLCubemapBuffer : public CubemapBuffer {
    void Populate(Cubemap *cubemap) override;
};


#endif
