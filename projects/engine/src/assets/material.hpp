#ifndef MANTA_MATERIAL_HPP
#define MANTA_MATERIAL_HPP

#include <vector>
#include <string>
#include <unordered_map>

class MaterialValue;

class Shader;
class Resources;
class World;

#define RENAME_BUFFER_LEN 256

// Defines some basic and universal properties almost every shader will end up using
// Plus supports the ability for the user to slap their own bindings on top
class Material {
public:
    // Initializes some of the engine defaults by default, otherwise the user can disable that
    Material(std::string name, Shader *shader, bool usingDefaults = true);

    void RegisterValue(MaterialValue *value);

    void Bind();

    // Returns a bool because if we rename it we can't continue iterating within the for loop otherwise we segfault!
    bool DrawImGui(World *world, Resources* resources);

    std::string name;
    Shader *pShader;
    std::vector<MaterialValue *> values;

    static Material *errorMaterial; // Fallback material, contains no inputs other than standard engine constants

    std::string renameBuffer;
};

class MaterialLoader {
public:
    Material* CreateMaterial(std::string name, Shader* shader, bool usingDefaults = true);

    std::unordered_map<std::string, Material *> materials;
};


#endif
