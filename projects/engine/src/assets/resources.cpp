#include "resources.hpp"

#include <filesystem>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <rendering/renderer.hpp>

#include <console/console.hpp>

#include <actors/actor.hpp>
#include <actors/world.hpp>

#include <components/engine/renderer.hpp>

#include <assets/material_values/material_value.hpp>

#include <iostream>

using namespace std::filesystem;

Resources::Resources() {
    FindDataPaths();
}

void Resources::Prewarm() {
    std::cout << "Prewarming engine content...\n";

    // Base models
    LoadModel("engine/base/models/cube.obj", "engine#cube");
    LoadModel("engine/base/models/quad.obj", "engine#quad");
    LoadModel("engine/base/models/plane.obj", "engine#plane");
    LoadModel("engine/base/models/sphere.obj", "engine#sphere");

    // Base textures
    LoadTexture("engine/base/textures/brdf_lut.png", "engine#brdf_lut");

    // Materials
    LoadMaterial("engine#standard", "engine/base/shaders/standard.glsl", "engine#shader#standard", true);
    LoadMaterial("engine#skybox", "engine/base/shaders/skybox.glsl", "engine#shader#skybox", true);
    LoadMaterial("engine#deferred_lighting", "engine/base/shaders/deferred_lighting.glsl", "engine#shader#deferred_lighting", true);
}

void IterateThroughPath(ResourcesPath &path) {
    for (auto &file: directory_iterator(path.path)) {
        if (file.is_directory()) {
            ResourcesPath subPath;
            subPath.path = file.path();

            IterateThroughPath(subPath);
            path.childPaths.emplace_back(subPath);
        } else {
            path.files.emplace_back(file);
            //printf("File: %s\n", file.path().c_str());
        }
    }
}

void Resources::FindDataPaths() {
    // Get surface level directories
    dataPaths.clear();

    ResourcesPath surfacePath;
    surfacePath.path = "./data";

    for (auto &file: directory_iterator(surfacePath.path)) {
        if (file.is_directory()) {
            ResourcesPath resPath;

            resPath.path = file.path();
            IterateThroughPath(resPath);
            surfacePath.childPaths.emplace_back(resPath);
        } else
            surfacePath.files.emplace_back(file);
    }

    dataPaths.emplace_back(surfacePath);
}

Model *Resources::LoadModel(std::string path, std::string id) {
    Model *model = modelLoader.LoadModel(path, id);

    if (model) {
        renderer->CreateVertexBuffer(model);
    }

    return model;
}

Shader *Resources::LoadShader(std::string path, std::string id) {
    Shader *shader = shaderLoader.LoadShader(path);

    if (shader)
        renderer->CreateShaderProgram(shader);

    return shader;
}

Texture *Resources::LoadTexture(std::string path, std::string id) {
    Texture *texture = textureLoader.LoadFromFile(path, id);

    if (texture)
        renderer->CreateTextureBuffer(texture);

    return texture;
}

Material *Resources::LoadMaterial(std::string name, std::string path, std::string id, bool usingDefaults) {
    // First we load the shader for the given material
    auto shader = LoadShader(path, id);

    if (shader) {
        return materialLoader.CreateMaterial(name, shader, true);
    }

    return nullptr;
}

void Resources::TryLoadFromExtension(std::string path, std::string extension) {
    if (extension == ".obj" || extension == ".mmdl")
        LoadModel(path);

    if (extension == ".glsl")
        LoadShader(path);

    if (extension == ".jpg" || extension == ".png" || extension == ".bmp")
        LoadTexture(path);
}

void DrawImGuiPaths(Resources *resources, ResourcesPath path) {
    std::string name = "UNKNOWN_FILE";

    if (path.path.string() == "./")
        name = "MANTA_ROOT";
    else
        name = path.path.filename().c_str();

    ImGui::PushID(path.path.c_str());
    if (ImGui::TreeNode(name.c_str())) {
        for (auto &subPath: path.childPaths) {
            DrawImGuiPaths(resources, subPath);
        }

        for (auto &file: path.files) {
            auto extension = file.extension();

            ImGui::PushID("load_");
            ImGui::PushID(path.path.c_str());
            if (ImGui::Button(file.filename().c_str())) {
                //printf("Path: %s\n", file.c_str());
                resources->TryLoadFromExtension(file.c_str(), extension.c_str());
            }
            ImGui::PopID();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void Resources::DrawImGuiWindow() {
    if (!showWindow)
        return;

    ImGui::Begin("Resources");

    ImGui::InputText("Path", &inputBuffer);

    if (ImGui::Button("Load Model##resources_window_load_model")) {
        LoadModel(inputBuffer);
    }

    if (ImGui::Button("Load Shader##resources_window_load_shader")) {
        LoadShader(inputBuffer);
    }

    if (ImGui::Button("Refresh explorer##resources_reload_explorer")) {
        FindDataPaths();
    }

    if (ImGui::TreeNode("Explorer##resources_window_explorer")) {
        for (auto &path: dataPaths)
            DrawImGuiPaths(this, path);

        ImGui::TreePop();
    }

    if (ImGui::Button("Refresh engine content##resources_reload_engine_content")) {
        Prewarm();
    }

    if (ImGui::TreeNode("Loaded Models##resources_window_loaded_models")) {
        for (auto &model: modelLoader.loadedModels) {
            if (!model.second)
                continue;

            const char *shaderName = "None";

            if (ImGui::TreeNode(model.second->name.c_str())) {
                ImGui::PushID(model.first.c_str());
                if (ImGui::Button(
                        "Create Actor##resources_window_loaded_models_create_ent_")) { // Creates an empty entity with this model inside it
                    AActor *modelEnt = new AActor();

                    auto renderer = new CRenderer();
                    renderer->AddModel(&model.second, &Material::errorMaterial);
                    modelEnt->AddComponent(renderer);

                    modelEnt->name = model.second->name;
                    world->actors.emplace_back(modelEnt);
                }
                ImGui::PopID();

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Loaded Shaders##resources_window_loaded_shaders")) {
        for (auto &shader: shaderLoader.shaders) {
            ImGui::PushID("resources_window_loaded_shaders_shader_entry");
            if (ImGui::TreeNode(shader.second->name.c_str())) {
                ImGui::PushID("resources_window_loaded_shaders_shader_entry_create_material");
                ImGui::PushID(shader.second->name.c_str());

                if (ImGui::Button("Create Material")) {
                    materialLoader.CreateMaterial("New Material", shader.second);
                }

                ImGui::PopID();
                ImGui::PopID();

                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Materials##resources_window_materials")) {
        for (auto &material: materialLoader.materials) {
            if (material.second->DrawImGui(world, this))
                break;
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Loaded Textures##resources_window_loaded_textures")) {
        for (auto &texture: textureLoader.loadedTextures) {
            ImGui::Text("%s", texture.second->name.c_str());
            ImGui::Image((void *) (intptr_t) texture.second->texBuffer->handle, ImVec2(100, 100));
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
