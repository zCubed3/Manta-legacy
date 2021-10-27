#include "actors/actor.hpp"
#include <actors/world.hpp>

#include <stdio.h>

#include <assets/resources.hpp>
#include <assets/model.hpp>

#include <actors/engine/light.hpp>

#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <imgui/misc/cpp/imgui_stdlib.h>

#include <console/console.hpp>

using namespace std::placeholders;

void World::CFunc_CreateEntity(Console *console, std::vector<std::string> args) {
    AActor *entity = new AActor();
    if (args.size() >= 1)
        entity->name = args[0];

    actors.emplace_back(entity);
}

void World::CreateConObjects(Console *console) {
    if (console) {
        //
        // CFuncs
        //
        console->CreateCFunc("ent_create", BIND_MEMBER_FUNC(&World::CFunc_CreateEntity, this));

        //console->Create
        //
        this->console = console;
    }
}

void World::Update() {
    int l = 0;
    for (int e = 0; e < actors.size(); e++) {
        if (actors[e] == nullptr) {
            printf("Actor #%i is a nullptr\n", e);
            continue;
        }

        ALight *light = dynamic_cast<ALight *>(actors[e]);

        actors[e]->Update(this);

        if (light != nullptr && actors[e]->isVisible) {
            data.lightPositions[l] = light->position;
            data.lightDirections[l] = glm::rotate(light->rotation, glm::vec3(0, 0, 1));
            data.lightColors[l] = light->color;
            data.lightRanges[l] = light->range;
            data.lightIntensities[l] = light->intensity;
            data.lightParams1[l] = light->param1;
            data.lightParams2[l] = light->param2;
            data.lightTypes[l] = (int) light->type;
            data.lights[l] = light;
            l++;
        }
    }

    data.lightCount = l;
}

void World::Draw(Renderer *renderer) {
    timeTotal = (float) glfwGetTime();
    timeDelta = timeTotal - timeLast;
    timeLast = timeTotal;

    for (int e = 0; e < actors.size(); e++) {
        if (actors[e] == nullptr)
            continue;

        actors[e]->Draw(renderer, resources);
    }
}

void World::DrawImGuiWindow(Resources* resources) {
    if (!showWindow)
        return;

    ImGui::Begin("World");

    if (ImGui::TreeNode("Create Actor##create_ent_menu")) {
        if (ImGui::Button("+ Light")) {
            ALight *light = new ALight();
            light->name = "New Light";
            actors.emplace_back(light);
        }

        if (ImGui::Button("+ Actor"))
            actors.emplace_back(new AActor());

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("World Settings")) {
        ImGui::DragFloat3("Ambient Color", glm::value_ptr(data.ambientColor), timeDelta, 0, 1);

        if (ImGui::TreeNode("World Info")) {
            ImGui::Text("Lights: %i", data.lightCount);
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }


    int deletion = -1;
    if (ImGui::TreeNode("Actors")) {
        int e = 0;
        for (const auto &entity: actors) {
            bool cantDrawX = false;

            if (entity)
                cantDrawX = entity->isProtected;

            if (!cantDrawX) {
                ImGui::PushID("delete_ent_");
                ImGui::PushID(e);
                if (ImGui::Button("X")) {
                    ImGui::PopID();
                    ImGui::PopID();
                    deletion = e;
                    break;
                }
                ImGui::PopID();
                ImGui::PopID();
            }

            if (entity == nullptr)
                continue;

            if (!cantDrawX)
                ImGui::SameLine();
            entity->DrawImGui(this, resources, e);
            ImGui::Spacing();

            e++;
        }
        ImGui::TreePop();
    }

    if (deletion >= 0)
        actors.erase(actors.begin() + deletion);

    ImGui::End();
}
