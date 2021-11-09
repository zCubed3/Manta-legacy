#include "material_texture.hpp"

#include <stdexcept>

#include "../shader.hpp"
#include "../texture.hpp"
#include <rendering/opengl/assets/glshader.hpp>

#include <GL/glew.h>

#include <imgui/imgui.h>

#include "../resources.hpp"

MaterialTexture::MaterialTexture(std::string location, Texture* value, int bindLocation) {
    this->location = location;
    this->texture = value;
    this->bindLocation = bindLocation;
}

void MaterialTexture::BindValue(Shader *pShader) {
    if (pShader == nullptr)
        throw std::runtime_error("Can't bind to a null shader!");

    // Ok this is dumb but whatever
    int actualBindLocation = 0;

    switch (bindLocation) {
        case 9:
            actualBindLocation = GL_TEXTURE9;
            break;

        case 10:
            actualBindLocation = GL_TEXTURE10;
            break;

        case 11:
            actualBindLocation = GL_TEXTURE11;
            break;
    }

    glActiveTexture(actualBindLocation);
    glBindTexture(GL_TEXTURE_2D, texture->texBuffer->handle);

    auto program = dynamic_cast<GLShaderProgram *>(pShader->program);
    program->setInt(location, bindLocation);
}

void MaterialTexture::DrawProperty(Resources* resources, World *world, std::string identifier) {
    ImGui::PushID("material_property_color_");
    ImGui::PushID(identifier.c_str());

    if (ImGui::TreeNode(location.c_str())) {
        ImGui::PushID("material_property_float_");
        ImGui::PushID(identifier.c_str());
        ImGui::PushID(location.c_str());

        ImGui::Image((void *) (intptr_t) texture->texBuffer->handle, ImVec2(100, 100));

        ImGui::PopID();
        ImGui::PopID();
        ImGui::PopID();

        if (ImGui::BeginCombo("##material_texture_property_editor", texture->name.c_str())) {
            int unique = 0;
            for (auto &loadedTexture: resources->textureLoader.loadedTextures) {
                ImGui::PushID("material_texture_property_editor_selection");
                ImGui::PushID(identifier.c_str());
                ImGui::PushID(unique);

                if (ImGui::Selectable(loadedTexture.second->name.c_str(), false)) {
                    texture = loadedTexture.second;
                }

                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();

                unique++;
            }

            ImGui::EndCombo();
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
    ImGui::PopID();
}