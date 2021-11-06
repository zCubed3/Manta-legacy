#ifndef MANTA_RESOURCES_H
#define MANTA_RESOURCES_H

// Combines several asset loaders into one structure

#include "model.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "cubemap.hpp"

#include <filesystem>

class Console;

class World;

class MEngine;

struct ResourcesPath {
    std::filesystem::path path; // This current path
    std::vector<ResourcesPath> childPaths;
    std::vector<std::filesystem::path> files;
};

class Resources {
public:
    Resources();

    void LoadBaseContent(MEngine* engine); // Loads default content

    void FindDataPaths();

    std::vector<ResourcesPath> dataPaths;

    ModelLoader modelLoader;
    ShaderLoader shaderLoader;
    TextureLoader textureLoader;
    MaterialLoader materialLoader;
    CubemapLoader cubemapLoader;

    Model *LoadModel(MEngine* engine, std::string path, std::string id = "");

    Shader *LoadShader(MEngine* engine, std::string path, std::string id = "");

    Texture *LoadTexture(MEngine* engine, std::string path, std::string id = "");

    Cubemap *CreateCubemap(MEngine* engine, std::string id);
    Cubemap *LoadCubemap(MEngine* engine, std::string path, std::string id = "");

    Material *LoadMaterial(MEngine* engine, std::string name, std::string path, std::string id = "", bool usingDefaults = true);
    Material *CreateMaterial(MEngine* engine, std::string name, std::string shader_id, bool usingDefaults = true);

    bool showWindow = false;

    void DrawImGuiWindow(MEngine* engine);

    void TryLoadFromExtension(MEngine* engine, std::string path, std::string extension);

    std::string inputBuffer;
};

#endif
