#ifndef MANTA_RESOURCES_H
#define MANTA_RESOURCES_H

// Combines several difference asset loaders into one structure

#include "model.hpp"
#include "shader.hpp"

class Resources {
   public:
      ModelLoader modelLoader;
      ShaderLoader shaderLoader;

      bool showWindow = false;
      void DrawImGuiWindow();

      Renderer* renderer;

      std::string inputBuffer;
};

#endif
