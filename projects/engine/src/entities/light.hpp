#ifndef MANTA_LIGHT_H
#define MANTA_LIGHT_H

#include "entity.hpp"

class LightData {
   public:
      glm::vec3 position;
      glm::vec3 rotation;
      glm::vec3 color;
      int type;
};

class Light : public Entity {
   public:
      glm::vec3 color = glm::vec3(1, 1, 1); 
      float range = 10.0f;
      float intensity = 1.0f;

      // Used for certain lighting types
      // Spot: 1 = inner angle, 2 = outer angle
      float param1 = 45.0f;
      float param2 = 55.0f;

      enum class LightType {
      	 Sun,
	 Point,
	 Spot
      };

      LightType type;
};

#endif
