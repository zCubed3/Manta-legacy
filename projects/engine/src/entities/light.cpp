#include "light.hpp"

#include <imgui/imgui.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "world.hpp"

#include <assets/texture.hpp>

Light::Light() {
    CreateShadowmap();
}

void Light::CreateShadowmap() {

}

void Light::Update(World *world) {
    Entity::Update(world);

    glm::mat4 view = glm::lookAt(position, position + glm::rotate(rotation, glm::vec3(0, 0, 1)), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.001f, 10.0f);
    mLightMatrix = projection * view;
}

void Light::DrawImGuiSub(World *world, int index) {
    ImGui::PushID("entity_editor_light");
    ImGui::PushID(index);
    if (ImGui::TreeNode("Light")) {

        ImGui::Text("Color:");
        ImGui::SameLine();
        ImGui::ColorEdit3("##color", glm::value_ptr(color), world->timeDelta);

        ImGui::Text("Range:");
        ImGui::SameLine();
        ImGui::DragFloat("##range", &range, world->timeDelta, 0);

        ImGui::Text("Intensity:");
        ImGui::SameLine();
        ImGui::DragFloat("##intensity", &intensity, world->timeDelta, 0);

        ImGui::Text("Param 1:");
        ImGui::SameLine();
        ImGui::DragFloat("##param1", &param1, world->timeDelta, 0);

        ImGui::Text("Param 2:");
        ImGui::SameLine();
        ImGui::DragFloat("##param2", &param2, world->timeDelta, 0);

        ImGui::Text("Type:");
        ImGui::SameLine();
        ImGui::InputInt("##type", reinterpret_cast<int *>(&type));

        ImGui::TreePop();
    }
    ImGui::PopID();
    ImGui::PopID();
}
