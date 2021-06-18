#ifndef MANTA_TEXTURE_H
#define MANTA_TEXTURE_H

#include <string>
#include <unordered_map>

class Texture {
   public:
      enum class TextureType {
	 Texture2D,
	 Texture3D
      };

   public:
      Texture(std::string name, int width, int height, TextureType type);

      int width, height;
      TextureType type;
      unsigned char* data;
      std::string name;
      //TextureBuffer* buffer;
};

class TextureLoader {
   public:
      Texture* CreateTexture(std::string name, int width, int height, Texture::TextureType type); 
      std::unordered_map<std::string, Texture*> loadedTextures;
};

#endif
