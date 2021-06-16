#ifndef MANTA_WORLD_H
#define MANTA_WORLD_H

#include <vector>
#include <glm/glm/glm.hpp>

class Entity;
class Light;
class Console;

#include <entities/light.hpp>

#define MAX_LIGHTS 32

class WorldData {
   public:
      glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);

      int lightCount;
      glm::vec3 lightPositions[MAX_LIGHTS];
      glm::vec3 lightColors[MAX_LIGHTS];
      int lightTypes[MAX_LIGHTS];
};

class World {
   public:
      std::vector<Entity*> entities;
      
      WorldData data;
      Console* console;

      void CreateConObjects(Console* console);
      void Update();
};

#endif
