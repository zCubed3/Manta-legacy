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

struct ResourcesPath {
    std::filesystem::path path; // This current path
    std::vector<ResourcesPath> childPaths;
    std::vector<std::filesystem::path> files;
};

class Resources {
public:
    Resources();

    void Prewarm(); // Loads default content

    void FindDataPaths();

    std::vector<ResourcesPath> dataPaths;

    ModelLoader modelLoader;
    ShaderLoader shaderLoader;
    TextureLoader textureLoader;
    MaterialLoader materialLoader;
    CubemapLoader cubemapLoader;

    Model *LoadModel(std::string path, std::string id = "");

    Shader *LoadShader(std::string path, std::string id = "");

    Texture *LoadTexture(std::string path, std::string id = "");

    Cubemap *CreateCubemap(std::string id);
    Cubemap *LoadCubemap(std::string path, std::string id = "");

    Material *LoadMaterial(std::string name, std::string path, std::string id = "", bool usingDefaults = true);

    bool showWindow = false;

    void DrawImGuiWindow();

    void TryLoadFromExtension(std::string path, std::string extension);

    Renderer *renderer;
    World *world;

    std::string inputBuffer;
};

#endif
