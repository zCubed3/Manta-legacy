#ifndef MANTA_ENTITY_H
#define MANTA_ENTITY_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#include <vector>
#include <string>

class Model;

class Renderer;

class World;

class Resources;

class CComponent;

// Because I kinda like UE4 terminology, Actors are prefixed with A, therefore, AActor!
class AActor {
public:
    std::string name = "New entity", nameBuffer = "";

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::vec3 euler = glm::vec3(0, 0, 0);
    glm::quat rotation;

    glm::mat4 mModel;
    glm::mat4 mModel_it; // Model matrice's inverse transpose (easier to calculate on the CPU than GPU)

    std::vector<Model **> models;

    std::vector<CComponent *> components;

    virtual void AddComponent(CComponent *pComponent);

    bool isEnabled = true;
    bool isVisible = true;
    bool isProtected = false; // Prevents deletion of this, useful for cameras and stuff

    virtual void Update(World *world);

    virtual void Draw(Renderer *renderer, Resources *resources);

    // This is usually ran inside of a tree
    void DrawImGui(World *world, Resources *resources, int index);

    virtual void
    DrawImGuiSub(World *world, int index) {}; // By default this does nothing, but is useful for lights and stuff
};

#endif
