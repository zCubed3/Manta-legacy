#ifndef MANTA_TEXTURE_H
#define MANTA_TEXTURE_H

#include <string>
#include <unordered_map>

class Texture;

class TextureBuffer {
   public:
      virtual void Populate(Texture* texture) = 0;
}; // For storing API data

class Texture {
   public:
      enum class TextureType {
	 Texture2D,
	 Texture3D
      };

      enum class Format {
	 Float, Byte
      };

      enum class Filtering {
	 Point,
	 Bilinear
      };

      enum class DataFormat {
	 RGBA_16F, RGBA
      };

   public:
      Texture(std::string name, int width, int height, TextureType type, Format format, Filtering filtering);

      void Resize(int newWidth, int newHeight);

      int width, height;
      TextureType type;

      Format format;
      Filtering filtering;
      DataFormat dataFormat = DataFormat::RGBA;
      bool isDepthMap = false;

      float* floatData = nullptr;
      unsigned char* byteData = nullptr;
      
      std::string name;
      
      TextureBuffer* texBuffer = nullptr;
};

class TextureLoader {
   public:
      Texture* CreateTexture(std::string name, int width, int height, Texture::TextureType type, Texture::Format format = Texture::Format::Byte, 
	    Texture::Filtering filtering = Texture::Filtering::Bilinear); 
      
      std::unordered_map<std::string, Texture*> loadedTextures;
};

#endif
