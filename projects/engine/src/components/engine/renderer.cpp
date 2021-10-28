#include "renderer.hpp"

#include <imgui/imgui.h>

#include <assets/resources.hpp>
#include <rendering/renderer.hpp>

void CRenderer::Update() {}

void CRenderer::Draw(Renderer *renderer, Resources *resources) {
    for (auto& model: models) {
        if (!model.first)
            return;

        if (!*model.first) {
            printf("Model is a nullptr!\n");
            continue;
        }

        (*model.first)->Draw(renderer, resources, owner, *model.second);
    }
}

// Just so we don't have to do std::pair constantly
void CRenderer::AddModel(Model** pModel, Material** pMaterial) {
    models.emplace_back(std::pair<Model**, Material**>(pModel, pMaterial));
}

void CRenderer::DrawImGuiWindowSub(World *world, Resources *resources, int index) {
    ImGui::PushID("entity_editor_component_renderer");
    ImGui::PushID(index);
    if (ImGui::TreeNode("CRenderer")) {
        ImGui::PushID("entity_editor_component_renderer_model_add_button");
        ImGui::PushID(index);
        if (ImGui::Button("+")) {
            AddModel(nullptr, &Material::errorMaterial);
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
                    bool isEmpty = model.first == nullptr;

                    if (!isEmpty)
                        isEmpty = *model.first == nullptr;

                    auto name = (isEmpty ? "None" : (*model.first)->name.c_str());
                    ImGui::Text("Model:");

                    ImGui::PushID("entity_editor_component_renderer_model_slot_combo");
                    ImGui::PushID(index);
                    if (ImGui::BeginCombo("##model_slot_model", name)) {
                        int unique = 0;
                        for (auto &loadedModel: resources->modelLoader.loadedModels) {
                            ImGui::PushID("entity_editor_component_renderer_model_slot_combo_entry");
                            ImGui::PushID(index);
                            ImGui::PushID(unique);

                            if (ImGui::Selectable(loadedModel.second->name.c_str(), false))
                                model.first = &loadedModel.second;

                            ImGui::PopID();
                            ImGui::PopID();
                            ImGui::PopID();
                        }

                        ImGui::EndCombo();
                    }
                    ImGui::PopID();
                    ImGui::PopID();

                    ImGui::Text("Material:");

                    ImGui::PushID("entity_editor_component_renderer_model_slot_combo_material");
                    ImGui::PushID(index);
                    if (ImGui::BeginCombo("##model_slot_model", (*model.second)->name.c_str())) {
                        int unique = 0;
                        for (auto &loadedMaterial: resources->materialLoader.materials) {
                            ImGui::PushID("entity_editor_component_renderer_model_slot_combo_entry");
                            ImGui::PushID(index);
                            ImGui::PushID(unique);

                            if (ImGui::Selectable(loadedMaterial.first.c_str(), false))
                                model.second = &loadedMaterial.second;

                            ImGui::PopID();
                            ImGui::PopID();
                            ImGui::PopID();
                        }

                        ImGui::EndCombo();
                    }
                    ImGui::PopID();
                    ImGui::PopID();

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