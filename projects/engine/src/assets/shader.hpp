#ifndef MANTA_SHADER_H
#define MANTA_SHADER_H

#include <unordered_map>
#include <string>

class Shader;
class Renderer;

class ShaderProgram {
   public:
      virtual void Compile(Shader* shader) = 0;
      virtual void Bind() = 0;

      // Unless a pragma is declared, these are not true...
      bool lightingEnabled; 	// #pragma MANTA_ENABLE_LIGHTING
      bool shadowsEnabled; 	// #pragma MANTA_ENABLE_SHADOWS
      bool globalsEnabled; 	// #pragma MANTA_ENABLE_GLOBALS

      virtual ~ShaderProgram() = 0;
};

class Shader {
   public:
      std::string name; // Is usually the path unless internal
      std::string code; // Usually cleared after compilation
      ShaderProgram* program;

      void Bind();
};

class ShaderLoader {
   public:
      Shader* LoadShader(std::string path);   
      Shader* LoadCode(std::string name, std::string code = "");

      std::unordered_map<std::string, Shader*> shaders;
};

#endif
