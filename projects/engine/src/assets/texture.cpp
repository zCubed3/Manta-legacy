#include "texture.hpp"

Texture::Texture(std::string name, int width, int height, TextureType type, Format format, Filtering filtering) {
   this->name = name;
   
   if (format == Format::Byte)
      byteData = new unsigned char[width * height];

   if (format == Format::Float)
      floatData = new float[width * height];

   this->width = width;
   this->height = height;
   this->type = type;
   this->format = format;
   this->filtering = filtering;
}

// Clears all the old data!
void Texture::Resize(int newWidth, int newHeight) {
   if (byteData)
      delete[] byteData;
   
   if (floatData)
      delete[] floatData;

   if (format == Format::Byte)
      byteData = new unsigned char[width * height];
   
   if (format == Format::Float)
      floatData = new float[width * height];

   width = newWidth;
   height = newHeight;
}

Texture* TextureLoader::CreateTexture(std::string name, int width, int height, Texture::TextureType type, Texture::Format format, Texture::Filtering filtering) {
   if (loadedTextures.count(name) > 0)
      return loadedTextures[name];

   Texture* texture = new Texture(name, width, height, type, format, filtering);
   loadedTextures.emplace(name, texture);
   return texture;
}
