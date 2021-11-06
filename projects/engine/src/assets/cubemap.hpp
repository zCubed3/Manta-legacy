#ifndef MANTA_CUBEMAP_HPP
#define MANTA_CUBEMAP_HPP

#include "texture.hpp"

class Cubemap;

// Literally a texture buffer but handles creating 3D textures
class CubemapBuffer {
public:
    // TODO virtual ~CubemapBuffer() {};

    virtual void Populate(Cubemap *cubemap) = 0;

    unsigned int handle; // Meant for ImGui but usually OGL and VK use handles
};

class Cubemap {
public:
    Cubemap(Texture *top, Texture *bottom, Texture *right, Texture *left, Texture *forward, Texture *back);

    Texture *top, *bottom, *right, *left, *forward, *back;

    CubemapBuffer *buffer;
};

class CubemapLoader {
public:
    std::unordered_map<std::string, Cubemap *> cubemaps;
};

#endif
