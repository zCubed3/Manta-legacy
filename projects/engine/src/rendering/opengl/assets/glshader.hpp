#ifndef MANTAGL3_GLSHADER_H
#define MANTAGL3_GLSHADER_H

#include <unordered_map>
#include <assets/shader.hpp>

typedef unsigned int uint;

class GL3ShaderProgram : public ShaderProgram {
   public:
      bool Compile(Shader* shader);
      void Bind();

      ~GL3ShaderProgram();

      std::unordered_map<std::string, int> uniforms;
      int getUniform(std::string name);

      uint program;
};

#endif
