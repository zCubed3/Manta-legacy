#ifndef MANTA_MATERIAL_VALUE_HPP
#define MANTA_MATERIAL_VALUE_HPP

#include <string>

class Shader;

class World;

class Resources;

class MaterialValue { // Abstract class that just holds material values and handles binding them to the rendering pipeline
public:
    std::string location;

    virtual void BindValue(Shader *pShader) = 0;

    // Takes an identifier to make it unique according to ImGui
    virtual void DrawProperty(Resources* resources, World *world, std::string identifier) = 0;
};

#endif
