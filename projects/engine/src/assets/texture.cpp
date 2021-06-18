#include "texture.hpp"

Texture::Texture(std::string name, int width, int height, TextureType type) {
   this->name = name;
   data = new unsigned char[width * height];

   this->width = width;
   this->height = height;
   this->type = type;
}

Texture* TextureLoader::CreateTexture(std::string name, int width, int height, Texture::TextureType type) {
   if (loadedTextures.count(name) >= 0)
      return loadedTextures[name];

   Texture* texture = new Texture(name, width, height, type);
   loadedTextures.emplace(name, texture);
   return texture;
}
