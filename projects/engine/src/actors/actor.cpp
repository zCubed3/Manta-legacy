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

#include <core/engine.hpp>

// Normally actors just update their matrices
void AActor::Update(MEngine *engine) {
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

    for (auto &component: components) {
        if (component->isVisible)
            component->Draw(renderer, resources);
    }
}

void AActor::DrawImGui(World *world, Resources *resources, int index) {
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

        DrawImGuiSub(world, index);

        for (auto &component: components)
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