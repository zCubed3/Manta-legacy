#ifndef MANTAGL3_GLSHADERPROGRAM_H
#define MANTAGL3_GLSHADERPROGRAM_H

#include <assets/shader.hpp>

typedef unsigned int uint;

class GL3ShaderProgram : public ShaderProgram {
   public:
      void Compile(Shader* shader);
      void Bind(Renderer* renderer);

      ~GL3ShaderProgram();

      uint program;
};

#endif
