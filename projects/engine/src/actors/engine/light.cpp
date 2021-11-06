#include "light.hpp"

#include <imgui/imgui.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "actors/world.hpp"

#include <assets/texture.hpp>

#include <core/engine.hpp>

#include <GL/glew.h>

void ALight::Start(MEngine *engine) {
    // Every light is given a random number, this hopefully won't ever collide
    unique_id = rand();
    CreateShadowmap(engine);
}

void ALight::CreateShadowmap(MEngine *engine) {
    if (shadowmap == nullptr) {
        char shadowmap_id[128];

        sprintf(shadowmap_id, "SHADOWMAP_%i");
        shadowmap = engine->resources.textureLoader.CreateTexture(std::string(shadowmap_id), shadowmapWidth,
                                                                  shadowmapHeight, Texture::Format::Float,
                                                                  Texture::Filtering::Point);
        shadowmap->isDepthMap = true;
    }

    shadowmap->width = shadowmapWidth;
    shadowmap->height = shadowmapHeight;

    engine->renderer->CreateTextureBuffer(shadowmap);
}

void ALight::Update(MEngine *engine) {
    AActor::Update(engine);

    glm::mat4 view = glm::lookAt(position, position + glm::rotate(rotation, glm::vec3(0, 0, 1)), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.001f, 10.0f);
    mLightMatrix = projection * view;
}

// TODO: Not rely on OpenGL entirely?
void ALight::CustomRender(MEngine *engine) {
    Renderer *renderer = engine->renderer;

    auto *oldCamera = engine->world.pCamera;
    //engine->world.pCamera = lightCamera;

    renderer->BeginRender(engine, Renderer::RenderType::Shadowmap);

    glActiveTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, shadowmap->texBuffer->handle);

    engine->world.Draw(engine);

    renderer->EndRender();
}

void ALight::DrawImGuiSub(World *world, int index) {
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
