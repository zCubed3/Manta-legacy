#include "material.hpp"

#include "shader.hpp"

#include <stdexcept>
#include <utility>

#include "material_values/material_float.hpp"
#include "material_values/material_color.hpp"

Material *Material::errorMaterial = nullptr;

Material::Material(std::string name, Shader *pShader, bool usingDefaults) {
    if (pShader == nullptr)
        throw std::runtime_error("Can't create a material using null shader!");

    this->pShader = pShader;
    this->name = std::move(name);

    if (usingDefaults) {
        RegisterValue(new MaterialColor("MANTA_COLOR_ALBEDO", glm::vec4(1, 1, 1, 1)));
    }
}

void Material::RegisterValue(MaterialValue *value) {
    this->values.emplace_back(value); // TODO Not make this as unsafe or shit
}

void Material::Bind() {
    pShader->Bind(); // TODO SAFETY!
}