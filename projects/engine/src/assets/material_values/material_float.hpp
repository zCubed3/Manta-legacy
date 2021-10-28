//
// Created by liam on 10/26/21.
//

#ifndef MANTA_MATERIAL_FLOAT_HPP
#define MANTA_MATERIAL_FLOAT_HPP

#include "material_value.hpp"

class MaterialFloat : public MaterialValue {
public:
    MaterialFloat(std::string location, float value);

    void BindValue(Shader *pShader) override;

    void DrawProperty(World *world, std::string identifier) override;

    float value;
};


#endif //MANTA_MATERIAL_FLOAT_HPP
