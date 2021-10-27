#ifndef MANTA_MATERIAL_HPP
#define MANTA_MATERIAL_HPP

#include <vector>
#include <string>

class MaterialValue;

class Shader;

// Defines some basic and universal properties almost every shader will end up using
// Plus supports the ability for the user to slap their own bindings on top
class Material {
public:
    // Initializes some of the engine defaults by default, otherwise the user can disable that
    Material(std::string name, Shader *shader, bool usingDefaults = true);

    void RegisterValue(MaterialValue *value);

    void Bind();

    std::string name;
    Shader *pShader;
    std::vector<MaterialValue *> values;

    static Material *errorMaterial; // Fallback material, contains no inputs other than standard engine constants
};


#endif
