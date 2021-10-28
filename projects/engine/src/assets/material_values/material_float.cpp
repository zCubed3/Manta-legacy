#include "material_float.hpp"

#include <stdexcept>

#include "../shader.hpp"
#include <rendering/opengl/assets/glshader.hpp>

#include <imgui/imgui.h>

#include <glm/glm/gtc/type_ptr.hpp>

#include <actors/world.hpp>

MaterialFloat::MaterialFloat(std::string location, float value) {
    this->location = location;
    this->value = value;
}

// TODO make this not depend on OpenGL entirely?
void MaterialFloat::BindValue(Shader *pShader) {
    if (pShader == nullptr)
        throw std::runtime_error("Can't bind to a null shader!");

    auto program = dynamic_cast<GLShaderProgram *>(pShader->program);
    program->setFloat(location, value);
}

void MaterialFloat::DrawProperty(World *world, std::string identifier) {
    ImGui::PushID("material_property_color_");
    ImGui::PushID(identifier.c_str());

    if (ImGui::TreeNode(location.c_str())) {
        ImGui::PushID("material_property_float_");
        ImGui::PushID(identifier.c_str());
        ImGui::PushID(location.c_str());

        ImGui::DragFloat("Value", &value, world->timeDelta);

        ImGui::PopID();
        ImGui::PopID();
        ImGui::PopID();

        ImGui::TreePop();
    }

    ImGui::PopID();
    ImGui::PopID();
}