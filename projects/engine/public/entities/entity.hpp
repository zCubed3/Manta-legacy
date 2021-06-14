#ifndef MANTA_ENTITY_H
#define MANTA_ENTITY_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#include <vector>

class Model;
class Renderer;

class Entity {
   public:
      glm::vec3 position = glm::vec3(0, 0, 0);
      glm::vec3 scale = glm::vec3(1, 1, 1);
      glm::vec3 euler = glm::vec3(0, 0, 0);
      glm::quat rotation;

      glm::mat4 mModel;

      std::vector<Model*> models;

      bool isEnabled = true;
      bool isVisible = true;

      virtual void Update();
      virtual void Draw(Renderer* renderer);
};

#endif
