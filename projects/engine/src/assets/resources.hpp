#ifndef MANTA_RESOURCES_H
#define MANTA_RESOURCES_H

// Combines several asset loaders into one structure

#include "model.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"

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

    void FindDataPaths();

    std::vector<ResourcesPath> dataPaths;

    ModelLoader modelLoader;
    ShaderLoader shaderLoader;
    TextureLoader textureLoader;
    MaterialLoader materialLoader;

    Model *LoadModel(std::string path);

    Shader *LoadShader(std::string path);

    Texture *LoadTexture(std::string path);

    bool showWindow = false;

    void DrawImGuiWindow();

    void TryLoadFromExtension(std::string path, std::string extension);

    Renderer *renderer;
    World *world;

    std::string inputBuffer;
};

#endif
