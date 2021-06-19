#ifndef MANTA_RESOURCES_H
#define MANTA_RESOURCES_H

// Combines several difference asset loaders into one structure

#include "model.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Resources {
   public:
      ModelLoader modelLoader;
      ShaderLoader shaderLoader;
      TextureLoader textureLoader;

      bool showWindow = false;
      void DrawImGuiWindow();

      Renderer* renderer;

      std::string inputBuffer;
};

#endif
