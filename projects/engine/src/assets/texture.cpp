#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

Texture::Texture(std::string name, int width, int height, Format format, Filtering filtering) {
    this->name = name;

    if (format == Format::Byte)
        byteData = new unsigned char[width * height];

    if (format == Format::Float)
        floatData = new float[width * height];

    this->width = width;
    this->height = height;
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

Texture *
TextureLoader::CreateTexture(std::string name, int width, int height, Texture::Format format,
                             Texture::Filtering filtering) {
    if (loadedTextures.count(name) > 0)
        return loadedTextures[name];

    Texture *texture = new Texture(name, width, height, format, filtering);
    loadedTextures.emplace(name, texture);
    return texture;
}

Texture *TextureLoader::LoadFromFile(std::string path, std::string id, Texture::Format format,
                                     Texture::Filtering filtering) {
    printf("Loading texture from %s\n", path.c_str());
    int width, height, channels;
    unsigned char *bytes = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (bytes == nullptr) {
        printf("Failed to read from %s\n", path.c_str());
        return nullptr;
    }

    Texture *texture = new Texture(path, width, height, format, filtering);
    delete texture->byteData;
    texture->byteData = bytes;

    switch (channels) {
        case 3:
            texture->dataFormat = Texture::DataFormat::RGB;
            break;

        case 4:
            texture->dataFormat = Texture::DataFormat::RGBA;
            break;
    }

    printf("Done loading texture!\n");

    std::string location = path;

    if (!id.empty())
        location = id;

    texture->name = location;
    loadedTextures.emplace(location, texture);
    return texture;
}

