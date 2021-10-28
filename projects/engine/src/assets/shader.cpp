#include "shader.hpp"

#include <iterator>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <chrono>
#include <utility>
#include "material.hpp"

ShaderProgram::~ShaderProgram() {} // Silence the compiler

void Shader::Bind() {
    if (program)
        program->Bind();
}

// Use GetShader
Shader *ShaderLoader::LoadShader(std::string path, std::string id) {
    if (!strstr(path.c_str(), ".glsl")) {
        printf("Provided file didn't end in .glsl, %s, ignoring it!\n", path.c_str());
        return nullptr;
    }

    std::ifstream file(path);

    if (!file.is_open()) {
        printf("Failed to open file at %s\n", path.c_str());
        return nullptr;
    }

    // Track the time taken to load
    auto start = std::chrono::high_resolution_clock::now();

    std::string location = path;

    if (!id.empty())
        location = id;

    Shader *buffer = GetShader(location);

    if (buffer == nullptr)
        buffer = new Shader();
    else {
        buffer->code.clear();
        buffer->name.clear();

        if (buffer->program) {
            printf("Shader already has a program, deleting it!\n");
            delete buffer->program;
            buffer->program = nullptr;
        }
    }

    std::string fileText((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer->code = fileText;
    buffer->name = path;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    printf("Loaded shader from %s, took %li ms\n", path.c_str(), duration.count());

    shaders.emplace(location, buffer);
    return buffer;
}

// Internally loaded shaders can't be reloaded for obvious reasons
Shader *ShaderLoader::LoadCode(std::string name, std::string code) {
    Shader *buffer = new Shader();
    buffer->code = code;
    buffer->name = name;

    printf("Loaded shader code internally, %s\n", name.c_str());
    shaders.emplace(name, buffer);
    return buffer;
}

Shader *ShaderLoader::GetShader(std::string name) {
    if (shaders.size() == 0)
        return nullptr;

    if (shaders.find(name) != shaders.end())
        return shaders[name];

    return nullptr;
}
