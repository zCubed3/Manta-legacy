#ifndef MANTAGL3_GL3API_H
#define MANTAGL3_GL3API_H

#include <renderer.hpp>

class GL3Renderer : public Renderer {
   public:
      void Initialize() { }; // Dummy
      RendererState Render() { RendererState state; return state; }; // Dummy
};

#endif
