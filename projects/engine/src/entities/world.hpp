#ifndef MANTA_WORLD_H
#define MANTA_WORLD_H

#include <vector>
#include <glm/glm/glm.hpp>

class Entity;
class Light;
class Console;
class Renderer;
class Resources;

#include <entities/light.hpp>

#define MAX_LIGHTS 32

class WorldData {
   public:
      glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);

      int lightCount;
      glm::vec3 lightPositions[MAX_LIGHTS];
      glm::vec3 lightDirections[MAX_LIGHTS];
      glm::vec3 lightColors[MAX_LIGHTS];
      float lightRanges[MAX_LIGHTS];
      float lightIntensities[MAX_LIGHTS];
      float lightParams1[MAX_LIGHTS];
      float lightParams2[MAX_LIGHTS];
      int lightTypes[MAX_LIGHTS];
};

class World {
   public:
      std::vector<Entity*> entities;
      
      WorldData data;
      Console* console;
      Resources* resources;

      void CreateConObjects(Console* console);
      void Update();
      void Draw(Renderer* renderer);

      float timeTotal = 0.0f;
      float timeLast = 0.0f;
      float timeDelta = 0.0f;

      bool showWindow = false;
      void DrawImGuiWindow();
};

#endif
