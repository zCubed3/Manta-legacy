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

      void Update();
};

#endif
