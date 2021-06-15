#include "model.hpp"

#include <string.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <chrono>

#include <assets/shader.hpp>
#include <renderer.hpp>

void Model::Draw(Renderer* renderer, Entity* entity) {
   if (renderer == nullptr) {
      printf("Can't Draw a model without a valid renderer!\n");
      return;
   }

   Shader* usedShader = shader;

   if (!usedShader) { // If we're missing a Shader, default to the error shader
      ShaderLoader loader = renderer->shaderLoader;

      usedShader = loader.LoadShader("engine#error");
      
      if (!usedShader) {
	 printf("Failed to fallback on the error shader!");
	 return;
      }
   }

   if (vertexBuffer != nullptr)
      vertexBuffer->Draw(renderer, entity, shader);
   else
      printf("Error: Model lacks a VertexBuffer, please assign it one!\n");
}

Model* ModelLoader::LoadModel(std::string path) {
   if (loadedModels.count(path) > 0) {
      if (loadedModels[path] != nullptr)
	 return loadedModels[path];
      else {
	 printf("Model in memory from %s was corrupt or unloaded, reloading it!\n", path.c_str());
	 loadedModels.erase(path);
      }
   }

   std::ifstream file(path);

   if (!file.is_open()) {
      printf("Failed to find a file at %s\n", path.c_str());
      return nullptr;
   }

   Model* buffer = new Model();
   
   // Track the time taken to load
   auto start = std::chrono::high_resolution_clock::now();

   // Not a very good OBJ loader but it works for our purposes right now
   if (strstr(path.c_str(), ".obj")) {
      std::vector<glm::vec3> positions;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec2> uv0s;
      std::vector<glm::ivec3> triangles;

      std::string line;
      while (std::getline(file, line)) {
	 // First two characters are a data id
	 std::string id = line.substr(0, 2);
	 std::string contents = line.substr(2);

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

	 // F = face, so we iterate 3 times to make a tri
	 // Ugly method of loading faces but idc enough to fix it
	 if (id[0] == 'f') {
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

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   
   printf("Loaded model from %s (%s), took %li ms\n", path.c_str(), buffer->name.c_str(), duration.count());

   loadedModels.emplace(path, buffer); // We keep models loaded as their paths to allow hot-reloading
   return buffer;
}

