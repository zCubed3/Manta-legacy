#ifndef MANTA_MATERIAL_VALUE_HPP
#define MANTA_MATERIAL_VALUE_HPP

#include <string>

class Shader;

class MaterialValue { // Abstract class that just holds material values and handles binding them to the rendering pipeline
public:
    std::string location;

    virtual void BindValue(Shader *pShader) = 0;
};

#endif
