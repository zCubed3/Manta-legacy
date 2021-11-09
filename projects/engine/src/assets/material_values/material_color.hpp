#ifndef MANTA_MATERIAL_COLOR_HPP
#define MANTA_MATERIAL_COLOR_HPP

#include "material_value.hpp"

#include <glm/glm/glm.hpp>

class MaterialColor : public MaterialValue {
public:
    MaterialColor(std::string location, glm::vec4 color);

    void BindValue(Shader *pShader) override;

    void DrawProperty(Resources* resources, World *world, std::string identifier) override;

    glm::vec4 color{};
};


#endif //MANTA_MATERIAL_COLOR_HPP
