#include "material.hpp"

#include "shader.hpp"

#include <stdexcept>

#include "material_values/material_float.hpp"
#include "material_values/material_color.hpp"

#include <imgui/imgui.h>

#include "resources.hpp"

Material *Material::errorMaterial = nullptr;

Material::Material(std::string name, Shader *pShader, bool usingDefaults) {
    if (pShader == nullptr)
        throw std::runtime_error("Can't create a material using null shader!");

    this->pShader = pShader;
    this->name = name;

    renameBuffer.resize(RENAME_BUFFER_LEN);

    if (usingDefaults) {
        RegisterValue(new MaterialFloat("MANTA_SCALAR_METALLIC", 0.0f));
        RegisterValue(new MaterialFloat("MANTA_SCALAR_ROUGHNESS", 0.0f));
        RegisterValue(new MaterialColor("MANTA_COLOR_ALBEDO", glm::vec4(1, 1, 1, 1)));
    }
}

void Material::RegisterValue(MaterialValue *value) {
    this->values.emplace_back(value); // TODO Not make this as unsafe or shit
}

void Material::Bind() {
    pShader->Bind(); // TODO SAFETY!

    for (auto& value : values) {
        value->BindValue(pShader);
    }
}

bool Material::DrawImGui(World *world, Resources* resources) {
    ImGui::PushID("resources_window_materials_entry");
    if (ImGui::TreeNode(name.c_str())) {
        if (ImGui::InputText("Name", renameBuffer.data(), RENAME_BUFFER_LEN, ImGuiInputTextFlags_EnterReturnsTrue)) {
            // Erase the old cache entry
            resources->materialLoader.materials.erase(name);

            name = std::string(renameBuffer);
            renameBuffer.clear();
            renameBuffer.resize(RENAME_BUFFER_LEN);

            // Move it back into the cache
            resources->materialLoader.materials.emplace(name, this);

            ImGui::TreePop();
            ImGui::PopID();
            return true;
        }

        for (auto& value: values) {
            value->DrawProperty(world, name);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

    return false;
}

// TODO: Safety
Material *MaterialLoader::CreateMaterial(std::string name, Shader* shader, bool usingDefaults) {
    auto mat = new Material(name, shader, usingDefaults);
    materials.emplace(name, mat);
    return mat;
}