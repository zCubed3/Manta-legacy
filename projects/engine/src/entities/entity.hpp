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

class Entity {
   public:
      std::string name = "New entity", nameBuffer = "";

      glm::vec3 position = glm::vec3(0, 0, 0);
      glm::vec3 scale = glm::vec3(1, 1, 1);
      glm::vec3 euler = glm::vec3(0, 0, 0);
      glm::quat rotation;

      glm::mat4 mModel;

      std::vector<Model**> models;

      bool isEnabled = true;
      bool isVisible = true;
      bool isProtected = false; // Prevents deletion of this, useful for cameras and stuff

      virtual void Update(World* world);
      virtual void Draw(Renderer* renderer, Resources* resources);
};

#endif
