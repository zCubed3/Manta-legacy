#ifndef MANTAGL3_GL3API_H
#define MANTAGL3_GL3API_H

#include <renderer.hpp>

typedef struct GLFWwindow GLFWwindow;

class GL3Renderer : public Renderer {
   public:
      void Initialize();
      RendererState Render();

   protected:
      GLFWwindow* window;
};

#endif