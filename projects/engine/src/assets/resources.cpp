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

#include <core/engine.hpp>

using namespace std::filesystem;

Resources::Resources() {
    FindDataPaths();
}

void Resources::LoadBaseContent(MEngine* engine) {
    std::cout << "Prewarming engine content...\n";

    // Base models
    LoadModel(engine, "engine/base/models/cube.obj", "engine#cube");
    LoadModel(engine, "engine/base/models/quad.obj", "engine#quad");
    LoadModel(engine, "engine/base/models/plane.obj", "engine#plane");
    LoadModel(engine, "engine/base/models/sphere.obj", "engine#sphere");

    // Base textures
    LoadTexture(engine, "engine/base/textures/white.png", "engine#white");
    LoadTexture(engine, "engine/base/textures/black.png", "engine#black");
    LoadTexture(engine, "engine/base/textures/brdf_lut.png", "engine#brdf_lut");

    // Base cubemaps
    CreateCubemap(engine, "engine#default_cubemap");

    // Materials
    LoadMaterial(engine, "engine#standard", "engine/base/shaders/standard.glsl", "engine#shader#standard", true);
    LoadMaterial(engine, "engine#skybox", "engine/base/shaders/skybox.glsl", "engine#shader#skybox", false);
    LoadMaterial(engine, "engine#deferred_lighting", "engine/base/shaders/deferred_lighting.glsl", "engine#shader#deferred_lighting", false);
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

Model *Resources::LoadModel(MEngine* engine, std::string path, std::string id) {
    Model *model = modelLoader.LoadModel(path, id);

    if (model) {
        engine->renderer->CreateVertexBuffer(model);
    }

    return model;
}

Shader *Resources::LoadShader(MEngine* engine, std::string path, std::string id) {
    Shader *shader = shaderLoader.LoadShader(path, id);

    if (shader)
        engine->renderer->CreateShaderProgram(shader);

    return shader;
}

Texture *Resources::LoadTexture(MEngine* engine, std::string path, std::string id) {
    Texture *texture = textureLoader.LoadFromFile(path, id);

    if (texture)
        engine->renderer->CreateTextureBuffer(texture);

    return texture;
}

// Returns a cubemap auto populated with engine#black
Cubemap *Resources::CreateCubemap(MEngine* engine, std::string id) {
    auto black = textureLoader.loadedTextures["engine#black"];

    if (black) {
        auto cubemap = new Cubemap(black, black, black, black, black, black);

        if (cubemap)
            engine->renderer->CreateCubemapBuffer(cubemap);

        cubemapLoader.cubemaps.emplace(id, cubemap);
        return cubemap;
    }

    return nullptr;
}

Cubemap *Resources::LoadCubemap(MEngine* engine, std::string path, std::string id) {
    Texture *base = textureLoader.LoadFromFile(path);

    if (base) {
        auto cubemap = new Cubemap(base, base, base, base, base, base);

        if (cubemap)
            engine->renderer->CreateCubemapBuffer(cubemap);

        return cubemap;
    }

    return nullptr;
}

Material *Resources::LoadMaterial(MEngine* engine, std::string name, std::string path, std::string id, bool usingDefaults) {
    // First we load the shader for the given material
    auto shader = LoadShader(engine, path, id);

    if (shader) {
        return materialLoader.CreateMaterial(name, shader, usingDefaults);
    }

    return nullptr;
}

Material *Resources::CreateMaterial(MEngine* engine, std::string name, std::string shader_id, bool usingDefaults) {
    auto shader = shaderLoader.shaders[shader_id];

    if (shader) {
        return materialLoader.CreateMaterial(name, shader, usingDefaults);
    }

    return nullptr;
}

void Resources::TryLoadFromExtension(MEngine* engine, std::string path, std::string extension) {
    if (extension == ".obj" || extension == ".mmdl")
        LoadModel(engine, path);

    if (extension == ".glsl")
        LoadShader(engine, path);

    if (extension == ".jpg" || extension == ".png" || extension == ".bmp")
        LoadTexture(engine, path);
}

void DrawImGuiPaths(MEngine* engine, ResourcesPath path) {
    std::string name = "UNKNOWN_FILE";

    if (path.path.string() == "./")
        name = "MANTA_ROOT";
    else
        name = path.path.filename().c_str();

    ImGui::PushID(path.path.c_str());
    if (ImGui::TreeNode(name.c_str())) {
        for (auto &subPath: path.childPaths) {
            DrawImGuiPaths(engine, subPath);
        }

        for (auto &file: path.files) {
            auto extension = file.extension();

            ImGui::PushID("load_");
            ImGui::PushID(path.path.c_str());
            if (ImGui::Button(file.filename().c_str())) {
                //printf("Path: %s\n", file.c_str());
                engine->resources.TryLoadFromExtension(engine, file.c_str(), extension.c_str());
            }
            ImGui::PopID();
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void DrawCubemapEditorSide(Resources* resources, Renderer* renderer, int side, std::string name, Cubemap* cubemap) {
    const char* side_name = "TODO";
    Texture** texture = nullptr;

    switch (side) {
        case 0:
            side_name = "Top";
            texture = &cubemap->top;
            break;

        case 1:
            side_name = "Bottom";
            texture = &cubemap->bottom;
            break;

        case 2:
            side_name = "Left";
            texture = &cubemap->left;
            break;

        case 3:
            side_name = "Right";
            texture = &cubemap->right;
            break;

        case 4:
            side_name = "Front";
            texture = &cubemap->forward;
            break;

        case 5:
            side_name = "Back";
            texture = &cubemap->back;
            break;
    }

    ImGui::Text("%s", side_name);

    if (!texture)
        return;

    ImGui::Image((void *)(intptr_t)(*texture)->texBuffer->handle, ImVec2(100, 100));

    ImGui::PushID(side);
    ImGui::PushID(name.c_str());

    if (ImGui::BeginCombo("##cubemap_editor_side", (*texture)->name.c_str())) {
        int unique = 0;
        for (auto &loadedTexture: resources->textureLoader.loadedTextures) {
            ImGui::PushID("cubemap_editor_side_tex_selection");
            ImGui::PushID(name.c_str());
            ImGui::PushID(unique);

            if (ImGui::Selectable(loadedTexture.second->name.c_str(), false)) {
                (*texture) = loadedTexture.second;

                renderer->CreateCubemapBuffer(cubemap);
            }

            ImGui::PopID();
            ImGui::PopID();
            ImGui::PopID();

            unique++;
        }

        ImGui::EndCombo();
    }

    ImGui::PopID();
    ImGui::PopID();
}

void DrawCubemapEditor(Resources* resources, Renderer* renderer, std::string name, Cubemap* cubemap) {
    ImGui::PushID("cubemap_editor");
    if (ImGui::TreeNode(name.c_str())) {
        for (int i = 0; i < 6; i++) {
            DrawCubemapEditorSide(resources, renderer, i, name, cubemap);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void Resources::DrawImGuiWindow(MEngine* engine) {
    if (!showWindow)
        return;

    ImGui::Begin("Resources");

    ImGui::InputText("Path", &inputBuffer);

    if (ImGui::Button("Load Model##resources_window_load_model")) {
        LoadModel(engine, inputBuffer);
    }

    if (ImGui::Button("Load Shader##resources_window_load_shader")) {
        LoadShader(engine, inputBuffer);
    }

    if (ImGui::Button("Refresh explorer##resources_reload_explorer")) {
        FindDataPaths();
    }

    if (ImGui::TreeNode("Explorer##resources_window_explorer")) {
        for (auto &path: dataPaths)
            DrawImGuiPaths(engine, path);

        ImGui::TreePop();
    }

    if (ImGui::Button("Refresh engine content##resources_reload_engine_content")) {
        LoadBaseContent(engine);
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
                    renderer->AddModel(model.second, Material::errorMaterial);
                    modelEnt->AddComponent(renderer);

                    modelEnt->name = model.second->name;
                    engine->world.actors.emplace_back(modelEnt);
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
            if (material.second->DrawImGui(&engine->world, this))
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

    if (ImGui::TreeNode("Loaded Cubemaps##resources_window_loaded_cubemaps")) {
        for (auto &cubemap: cubemapLoader.cubemaps) {
            DrawCubemapEditor(this, engine->renderer, cubemap.first, cubemap.second);
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
