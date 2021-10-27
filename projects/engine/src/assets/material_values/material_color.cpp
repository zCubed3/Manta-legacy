#include "material_color.hpp"

#include <utility>
#include <stdexcept>

#include "../shader.hpp"
#include <rendering/opengl/assets/glshader.hpp>

MaterialColor::MaterialColor(std::string location, glm::vec4 color) {
    this->location = std::move(location);
    this->color = color;
}

// TODO make this not depend on OpenGL entirely?
void MaterialColor::BindValue(Shader *pShader) {
    if (pShader == nullptr)
        throw std::runtime_error("Can't bind to a null shader!");

    auto program = dynamic_cast<GLShaderProgram *>(pShader->program);
    program->setVec4(location, color);
}