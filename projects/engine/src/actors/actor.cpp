#include "actor.hpp"
#include "world.hpp"

#include <stdio.h>

#include <assets/model.hpp>
#include <assets/resources.hpp>
#include <assets/material.hpp>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <glm/glm/gtc/type_ptr.hpp>

#include <components/component.hpp>

void AActor::Update(World *world) {
    if (!isEnabled)
        return;

    rotation = glm::quat(glm::radians(euler));

    mModel = glm::translate(glm::mat4(1.0f), position);
    mModel *= glm::toMat4(rotation);
    mModel = glm::scale(mModel, scale);

    mModel_it = glm::transpose(glm::inverse(mModel));
}

void AActor::Draw(Renderer *renderer, Resources *resources) {
    if (!isVisible)
        return;

    for (auto & component : components) {
        if (component->isVisible)
            component->Draw(renderer, resources);
    }
}

void AActor::DrawImGui(World *world, Resources* resources, int index) {
    ImGui::PushID("entity_editor_");
    ImGui::PushID(index);
    if (ImGui::TreeNode(name.c_str())) {

        ImGui::PushID(index);
        ImGui::Checkbox("##entity_editor_enabled_toggle_", &isEnabled);
        ImGui::PopID();

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::PushID("entity_editor_name");
        ImGui::PushID(index);
        if (ImGui::InputText("##name", &nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
            name = nameBuffer;
            nameBuffer.clear();
            nameBuffer.resize(0);
        }
        ImGui::PopID();
        ImGui::PopID();

        ImGui::PushID("entity_editor_transform");
        ImGui::PushID(index);
        if (ImGui::TreeNode("Transform")) {
            ImGui::Text("Position:");
            ImGui::SameLine();
            ImGui::DragFloat3("##position", glm::value_ptr(position), world->timeDelta);

            ImGui::Text("Euler:");
            ImGui::SameLine();
            ImGui::DragFloat3("##euler", glm::value_ptr(euler), world->timeDelta);

            ImGui::Text("Scale:");
            ImGui::SameLine();
            ImGui::DragFloat3("##scale", glm::value_ptr(scale), world->timeDelta);

            ImGui::TreePop();
        }
        ImGui::PopID();
        ImGui::PopID();

        ImGui::PushID("entity_editor_models_add");
        ImGui::PushID(index);
        if (ImGui::Button("+"))
            models.emplace_back(nullptr);
        ImGui::PopID();
        ImGui::PopID();

        /*
        ImGui::SameLine();
        ImGui::PushID("entity_editor_models");
        ImGui::PushID(index);
        if (ImGui::TreeNode("Models")) {
            int m = 0;
            for (auto &model: models) {
                const char *modelName = "None";
                if (model != nullptr)
                    if ((*model) != nullptr)
                        modelName = (*model)->name.c_str();

                ImGui::PushID("entity_editor_models_del_entry");
                ImGui::PushID(m);
                ImGui::PushID("_");
                ImGui::PushID(index);
                if (ImGui::Button("X")) {
                    models.erase(models.begin() + m);
                    ImGui::PopID();
                    ImGui::PopID();
                    ImGui::PopID();
                    ImGui::PopID();
                    break;
                }
                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();

                ImGui::SameLine();
                ImGui::Text("Model:");
                ImGui::SameLine();

                ImGui::PushID("entity_editor_models_list");
                ImGui::PushID(index);
                ImGui::PushID(m);
                if (ImGui::BeginCombo("##model", modelName)) {
                    int l = 0;

                    for (auto &loadedModel: world->resources->modelLoader.loadedModels) {
                        if (!loadedModel.second)
                            continue;

                        bool isSelected = modelName == loadedModel.second->name;

                        ImGui::PushID("entity_editor_models_list_entry");
                        ImGui::PushID(index);
                        ImGui::PushID(m);
                        if (ImGui::Selectable(loadedModel.second->name.c_str(), isSelected))
                            model = &loadedModel.second;
                        ImGui::PopID();
                        ImGui::PopID();
                        ImGui::PopID();

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();

                        l++;
                    }

                    m++;
                    ImGui::EndCombo();
                }
                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
        ImGui::PopID();
         */

        DrawImGuiSub(world, index);

        for (auto& component : components)
            component->DrawImGuiWindowSub(world, resources, index);

        ImGui::TreePop();
    }
    ImGui::PopID();
    ImGui::PopID();

}

void AActor::AddComponent(CComponent *pComponent) {
    pComponent->owner = this;
    components.emplace_back(pComponent); // TODO Safety!
}