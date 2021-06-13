#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <renderer.hpp>

typedef struct GLFWwindow GLFWwindow;

class GL3Renderer : public Renderer {
   public:
      void Initialize() override;
      RendererState Render() override;

      void CreateBuffer(Model* model) override;
      void CreateShaderProgram(Shader* shader) override;

      virtual void RegisterConObjects() override;

   protected:
      GLFWwindow* window;
};

#endif
