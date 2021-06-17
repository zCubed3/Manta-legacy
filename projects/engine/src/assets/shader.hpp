#ifndef MANTA_SHADER_H
#define MANTA_SHADER_H

#include <map>
#include <string>

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
      std::string name; // Is usually the path unless internal
      std::string code; // Usually cleared after compilation
      ShaderProgram* program;

      void Bind();
};

class ShaderLoader {
   public:
      Shader* LoadShader(std::string path);   
      Shader* LoadCode(std::string name, std::string code = "");

      std::map<std::string, Shader*> shaders;
};

#endif
