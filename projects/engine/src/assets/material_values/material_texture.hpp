#ifndef MANTA_MATERIAL_TEXTURE_HPP
#define MANTA_MATERIAL_TEXTURE_HPP

#include "material_value.hpp"

class Texture;

class MaterialTexture : public MaterialValue {
public:
    Texture* texture;
    int bindLocation;

    MaterialTexture(std::string location, Texture* value, int bindLocation);

    void BindValue(Shader *pShader) override;

    void DrawProperty(Resources* resources, World *world, std::string identifier) override;
};


#endif
