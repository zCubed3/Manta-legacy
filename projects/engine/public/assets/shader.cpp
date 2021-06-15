#include "shader.hpp"

#include <iterator>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <chrono>

void Shader::Bind() {
   if (program != nullptr)
      program->Bind();
   else
      printf("Error: Can't bind Shader, the ShaderProgram hasn't been assigned yet!\n");
}

Shader* ShaderLoader::LoadShader(std::string path) {
   if (shaders.count(path) > 0) {
      if (shaders[path] != nullptr)
	 return shaders[path];
      else {
	 printf("Loaded shader from %s was either deleted or corrupted, reloading it!\n", path.c_str());
      }
   }

   std::ifstream file(path);

   if (!file.is_open()) {
      printf("Failed to open file at %s\n", path.c_str());
      return nullptr;
   }

   // Track the time taken to load
   auto start = std::chrono::high_resolution_clock::now();

   Shader* buffer = new Shader();

   std::string fileText((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
   buffer->code = fileText;

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

   printf("Loaded shader from %s, took %li ms\n", path.c_str(), duration.count());
   shaders.emplace(path, buffer);
   return buffer;
}

Shader* ShaderLoader::LoadCode(std::string name, std::string code) { 
   if (shaders.count(name) > 0) {
      return shaders[name];
   }

   Shader* buffer = new Shader();
   buffer->code = code;

   printf("Loaded shader code internally, %s\n", name.c_str());
   shaders.emplace(name, buffer);
   return buffer;
}
