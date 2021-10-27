#include "renderer.hpp"

#include <imgui/imgui.h>

#include <assets/resources.hpp>
#include <rendering/renderer.hpp>

void CRenderer::Update() {}

void CRenderer::Draw(Renderer *renderer, Resources *resources) {
    for (int m = 0; m < models.size(); m++) {
        if (!models[m])
            continue;

        if (!*models[m]) {
            printf("Model #%i in entity is a nullptr!\n", m);
            continue;
        }

        (*models[m])->Draw(renderer, resources, owner, Material::errorMaterial);
    }
}

void CRenderer::DrawImGuiWindowSub(World *world, Resources *resources, int index) {
    ImGui::PushID("entity_editor_component_renderer");
    ImGui::PushID(index);
    if (ImGui::TreeNode("CRenderer")) {
        ImGui::PushID("entity_editor_component_renderer_model_add_button");
        ImGui::PushID(index);
        if (ImGui::Button("+")) {
            models.emplace_back(nullptr);
        }
        ImGui::PopID();
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID("entity_editor_component_renderer_models");
        ImGui::PushID(index);
        if (ImGui::TreeNode("Models")) {
            int deletion = -1;
            int i = 0;
            for (auto &model: models) {
                bool isEmpty = model == nullptr;

                ImGui::PushID("entity_editor_component_renderer_model_remove_button");
                ImGui::PushID(index);
                if (ImGui::Button("X")) {
                    deletion = i;
                    ImGui::PopID();
                    ImGui::PopID();
                    break;
                }
                ImGui::PopID();
                ImGui::PopID();

                ImGui::SameLine();

                ImGui::PushID("entity_editor_component_renderer_model_group");
                ImGui::PushID(index);

                char buffer[64];
                sprintf(buffer, "Model Slot %i", i);

                if (ImGui::TreeNode(buffer)) {
                    for (auto &model: resources->modelLoader.loadedModels) {

                    }

                    for (auto &material: resources->materialLoader.materials) {

                    }

                    ImGui::TreePop();
                }

                ImGui::PopID();
                ImGui::PopID();

                i++;
            }

            ImGui::TreePop();

            if (deletion >= 0) {
                models.erase(models.begin() + deletion);
            }
        }
        ImGui::PopID();
        ImGui::PopID();


        ImGui::TreePop();
    }
    ImGui::PopID();
    ImGui::PopID();

    // TODO: Recreate this behaviour with CMesh
    //if (ImGui::BeginCombo("Shader", shaderName)) {
    //    for (auto &shader: shaderLoader.shaders) {
    //        if (!shader.second)
    //            continue;
//
    //        bool isSelected = false;
//
    //        if (model.second->shader)
    //            isSelected = model.second->shader == shader.second;
//
    //        if (ImGui::Selectable(shader.first.c_str(), isSelected))
    //            model.second->shader = shader.second;
//
    //        if (isSelected)
    //            ImGui::SetItemDefaultFocus();
    //    }
//
    //    ImGui::EndCombo();
    //}
}