#include "shader.hpp"

#include <iterator>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

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

   Shader* buffer = new Shader();

   const char* cpath = path.c_str();
   if (strstr(cpath, ".glsl") || strstr(cpath, ".gl"))
      buffer->language = ShaderLanguage::GLSL;

   if (strstr(cpath, ".spirv") || strstr(cpath, ".v"))
      buffer->language = ShaderLanguage::SPIRV;

   if (strstr(cpath, ".hlsl") || strstr(cpath, ".hl"))
      buffer->language = ShaderLanguage::HLSL;

   std::string fileText((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
   buffer->code = fileText;

   shaders.emplace(path, buffer);
   return buffer;
}

Shader* ShaderLoader::LoadCode(std::string name, std::string code) { 
   if (shaders.count(name) > 0) {
      return shaders[name];
   }

   Shader* buffer = new Shader();
   buffer->code = code;

   shaders.emplace(name, buffer);
   return buffer;
}
