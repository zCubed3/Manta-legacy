#include "material_color.hpp"

#include <utility>
#include <stdexcept>

#include "../shader.hpp"
#include <rendering/opengl/assets/glshader.hpp>

#include <imgui/imgui.h>

#include <glm/glm/gtc/type_ptr.hpp>

#include <actors/world.hpp>

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

void MaterialColor::DrawProperty(Resources* resources, World *world, std::string identifier) {
    ImGui::PushID("material_property_color_");
    ImGui::PushID(identifier.c_str());

    if (ImGui::TreeNode(location.c_str())) {
        ImGui::PushID("material_property_color_");
        ImGui::PushID(identifier.c_str());
        ImGui::PushID(location.c_str());

        ImGui::ColorEdit4("Value", glm::value_ptr(color));

        ImGui::PopID();
        ImGui::PopID();
        ImGui::PopID();

        ImGui::TreePop();
    }

    ImGui::PopID();
    ImGui::PopID();
}