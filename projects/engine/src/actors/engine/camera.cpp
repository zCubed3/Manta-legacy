#include "camera.hpp"

#include "glm/glm/ext/matrix_transform.hpp"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/quaternion.hpp>

#include <rendering/renderer.hpp>
#include <console/console.hpp>

#include <imgui/imgui.h>

#include <core/engine.hpp>

void ACamera::Update(MEngine *engine) {
    AActor::Update(engine);

    glm::vec3 forward = glm::rotate(rotation, glm::vec3(0, 0, 1));

    mView = glm::lookAt(position, position + forward, glm::rotate(rotation, glm::vec3(0, 1, 0)));

    if (!ignoreConFov)
        if (engine)
            fieldOfView = engine->console.CVarGetInt("fov", 90);

    if (engine->renderer != nullptr)
        mPerspective = glm::perspective(glm::radians(fieldOfView),
                                        engine->renderer->windowWidth / engine->renderer->windowHeight,
                                        nearClip, farClip);
}

void ACamera::DrawImGuiSub(World *world, int index) {
    ImGui::PushID("entity_editor_light");
    ImGui::PushID(index);
    if (ImGui::TreeNode("Camera")) {
        ImGui::Text("FOV:");
        ImGui::SameLine();
        ImGui::DragFloat("##field_of_view", &fieldOfView, world->timeDelta, 0);

        ImGui::Text("Near Cull:");
        ImGui::SameLine();
        ImGui::DragFloat("##cull_near", &nearClip, world->timeDelta, 0);

        ImGui::Text("Far Cull:");
        ImGui::SameLine();
        ImGui::DragFloat("##cull_far", &farClip, world->timeDelta, 0);

        ImGui::Text("Ignore FOV CVar:");
        ImGui::SameLine();
        ImGui::Checkbox("##ignore_fov_cvar", &ignoreConFov);

        ImGui::TreePop();
    }
    ImGui::PopID();
    ImGui::PopID();
}
