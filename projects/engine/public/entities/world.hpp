#ifndef MANTA_WORLD_H
#define MANTA_WORLD_H

#include <vector>
#include <glm/glm/glm.hpp>

class Entity;
class Light;

#include <entities/light.hpp>

#define MAX_LIGHTS 32

typedef struct Float3_T {
   float x;
   float y;
   float z;
} Float3;

class WorldData {
   public:
      int lightCount;
      std::vector<glm::vec3> lightPositions;
};

class World {
   public:
      std::vector<Entity*> entities;
      
      WorldData data;

      void Update();
};

#endif
