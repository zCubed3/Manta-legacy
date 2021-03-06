#include "model.hpp"

#include <string.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <chrono>
#include <regex>

#include "shader.hpp"

#include <rendering/renderer.hpp>

#include <assets/resources.hpp>

void Model::Draw(MEngine *engine, AActor *entity, Material *material) {
    //Shader *usedShader = shader;

    //if (!usedShader) { // If we're missing a Shader, default to the error shader
    //    ShaderLoader loader = resources->shaderLoader;
    //    usedShader = loader.GetShader("engine#error");

    //    if (!usedShader) {
    //        printf("Failed to fallback on the error shader!");
    //        return;
    //    }
    //}

    if (vertexBuffer)
        vertexBuffer->Draw(engine, entity, material);
    else
        printf("Error: Model lacks a VertexBuffer, please assign it one!\n");
}

Model *ModelLoader::LoadModel(std::string path, std::string id) {
    std::ifstream file(path);

    if (!file.is_open()) {
        printf("Failed to find a file at %s\n", path.c_str());
        return nullptr;
    }

    Model *buffer = GetModel(path);
    bool recognized = false;
    bool exists = buffer != nullptr;

    if (buffer == nullptr)
        buffer = new Model();
    else {
        printf("A model loaded from %s already exists, replacing its contents!\n", path.c_str());
        buffer->vertices.clear();
        buffer->vertices.resize(0);

        buffer->triangles.clear();
        buffer->triangles.resize(0);

        if (buffer->vertexBuffer) {
            printf("Model already has a vertex buffer, deleting it!\n");
            delete buffer->vertexBuffer;
            buffer->vertexBuffer = nullptr;
        }
    }

    // Track the time taken to load
    auto start = std::chrono::high_resolution_clock::now();
    printf("Loading model\n");
    // Not a very good OBJ loader but it works for our purposes right now
    if (strstr(path.c_str(), ".obj")) {
        recognized = true;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv0s;
        std::vector<glm::ivec3> triangles;

        std::string line;
        while (std::getline(file, line)) {
            if (line.size() <= 2)
                continue;

            // First two characters are a data id
            std::string id = line.substr(0, 2);
            std::string contents = line.substr(2);

            if (id[0] == '#')
                continue;

            if (id[0] == 'o')
                buffer->name = contents;

            if (id[0] == 'v') {
                glm::vec3 v3data;

                if (id[1] == 't') {
                    glm::vec2 uv;
                    sscanf(contents.c_str(), "%f %f", &uv.x, &uv.y);
                    uv0s.emplace_back(uv);
                    continue;
                }

                sscanf(contents.c_str(), "%f %f %f", &v3data.x, &v3data.y, &v3data.z);

                if (id[1] == 'n')
                    normals.emplace_back(v3data);
                else
                    positions.emplace_back(v3data);
            }

            // F = face, we either iterate 3 times to make a tri, or once to make it a vert, depends on the obj
            // Ugly method of loading faces but idc enough to fix it
            //
            // Thanks to https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits for the digit checker
            if (id[0] == 'f') {
                // THIS DOESN'T WORK, NEEDS TO BE FIXED!
                if (std::all_of(contents.begin(), contents.end(), ::isdigit)) {
                    glm::ivec3 i1;

                    sscanf(contents.c_str(), "%i %i %i", &i1.x, &i1.y, &i1.z);

                    i1 - +glm::ivec3(1);
                    triangles.emplace_back(i1);
                } else {
                    glm::ivec3 i1, i2, i3;

                    sscanf(contents.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
                           &i1.x, &i1.y, &i1.z,
                           &i2.x, &i2.y, &i2.z,
                           &i3.x, &i3.y, &i3.z
                    );

                    glm::ivec3 one(1, 1, 1);

                    i1 -= one;
                    i2 -= one;
                    i3 -= one;

                    triangles.emplace_back(i1);
                    triangles.emplace_back(i2);
                    triangles.emplace_back(i3);
                }
            }
        }

        for (int t = 0; t < triangles.size(); t++) {
            glm::ivec3 tri = triangles[t];
            Model::Vertex vert;

            vert.position = positions[tri.x];
            vert.uv0 = uv0s[tri.y];
            vert.normal = normals[tri.z];

            bool similarVert = false;
            for (int v = 0; v < buffer->vertices.size(); v++)
                if (buffer->vertices[v] == vert) {
                    buffer->triangles.emplace_back(v);
                    similarVert = true;
                    break;
                }

            if (!similarVert) {
                buffer->triangles.emplace_back(buffer->vertices.size());
                buffer->vertices.emplace_back(vert);
            }
        }
    }

    if (strstr(path.c_str(), ".mmdl")) {
        recognized = true;
        char name[256];
        file.read(name, sizeof(name));

        buffer->name = name;

        int count;
        file.read(reinterpret_cast<char *>(&count), sizeof(int));

        for (int v = 0; v < count; v++) {
            float data[8];
            file.read(reinterpret_cast<char *>(data), sizeof(float) * 8);

            Model::Vertex vertex;

            vertex.position.x = data[0];
            vertex.position.y = data[1];
            vertex.position.z = data[2];

            vertex.normal.x = data[3];
            vertex.normal.y = data[4];
            vertex.normal.z = data[5];

            vertex.uv0.x = data[6];
            vertex.uv0.y = data[7];

            buffer->vertices.emplace_back(vertex);
        }

        file.read(reinterpret_cast<char *>(&count), sizeof(int));

        for (int t = 0; t < count; t++) {
            int data;
            file.read(reinterpret_cast<char *>(&data), sizeof(int));
            buffer->triangles.emplace_back(data);
        }
    }

    if (recognized) {
        std::string location = path;

        if (!id.empty())
            location = id;

        if (!exists)
            loadedModels.emplace(location, buffer);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        printf("\rLoaded model from %s (%s), took %li ms\n", path.c_str(), buffer->name.c_str(), duration.count());
    } else {
        printf("Model format wasn't supported, failed to load model!\n");
        delete buffer;
        return nullptr;
    }

    return buffer;
}

Model *
ModelLoader::CreateModel(std::string name, std::vector<Model::Vertex> vertices, std::vector<unsigned int> triangles) {
    Model *buffer = new Model();

    buffer->name = name;
    buffer->vertices = vertices;
    buffer->triangles = triangles;

    printf("Created model using internal data, named, %s\n", name.c_str());
    loadedModels.emplace(name, buffer);
    return buffer;
}

Model *ModelLoader::GetModel(std::string name) {
    if (loadedModels.size() == 0)
        return nullptr;

    if (loadedModels.find(name) != loadedModels.end())
        return loadedModels[name];
    else
        return nullptr;
}
