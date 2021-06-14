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
      glm::vec3 color; 

      enum class LightType {
      	 Sun,
	 Point
      };

      LightType type;
};

#endif
