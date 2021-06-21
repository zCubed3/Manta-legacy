#ifndef MANTA_TEXTURE_H
#define MANTA_TEXTURE_H

#include <string>
#include <unordered_map>

class Texture;

class TextureBuffer {
   public:
      virtual ~TextureBuffer() {};
      virtual void Populate(Texture* texture) = 0;

      unsigned int handle; // Meant for ImGui but usually OGL and VK use handles
}; // For storing API data

class Texture {
   public:
      enum class TextureType {
	 Texture2D,
	 Texture3D
      };

      enum class Format {
	 Float, Byte, Internal
      };

      enum class Filtering {
	 Point,
	 Bilinear
      };

      enum class DataFormat {
	 RGB, RGBA, RGBA_16F
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
     
      Texture* LoadFromFile(std::string path, Texture::TextureType type, Texture::Format format = Texture::Format::Byte, Texture::Filtering filtering = Texture::Filtering::Point);

      std::unordered_map<std::string, Texture*> loadedTextures;
};

#endif
