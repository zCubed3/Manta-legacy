#ifndef MANTA_SHADER_H
#define MANTA_SHADER_H

#include <map>
#include <string>

enum class ShaderLanguage {
   GLSL,
   SPIRV, 
   HLSL
};

class Shader;
class Renderer;

class ShaderProgram {
   public:
      virtual void Compile(Shader* shader) = 0;
      virtual void Bind() = 0;

      virtual ~ShaderProgram() = 0;
};

class Shader {
   public:
      std::string code; // Usually cleared after compilation
      ShaderLanguage language;
      ShaderProgram* program;

      void Bind();
};

class ShaderLoader {
   public:
      Shader* LoadShader(std::string path);   
      Shader* LoadCode(std::string name, std::string code);

      std::map<std::string, Shader*> shaders;
};

#endif
