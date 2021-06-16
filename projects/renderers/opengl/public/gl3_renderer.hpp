#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <renderer.hpp>

typedef unsigned int uint;

typedef struct GLFWwindow GLFWwindow;

class GL3Renderer : public Renderer {
   public:
      virtual const char* get_APIName() override { return "OpenGL 3.3"; };

      void Initialize() override;
      Status Render() override;

      void CreateBuffer(Model* model) override;
      void CreateShaderProgram(Shader* shader) override;

      virtual void CreateConObjects(Console* console) override;
};

extern std::string gl3ErrorShaderCode;

#endif
