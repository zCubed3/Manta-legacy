#include <entities/world.hpp>

#include <stdio.h>

#include <entities/light.hpp>

#include <glm/glm/gtc/type_ptr.hpp>

void World::Update() {

   int l = 0;
   for (int e = 0; e < entities.size(); e++) {
      if (entities[e] == nullptr) {
	 printf("Entity #%i is a nullptr\n", e);
	 continue;
      }

      Light* light = dynamic_cast<Light*>(entities[e]);

      if (light != nullptr) {
	 data.lightPositions[l] = light->position;
	 data.lightColors[l] = light->color;
	 data.lightTypes[l] = (int)light->type;
	 l++;	 
      }	 

      entities[e]->Update();
   }

   data.lightCount = l;
}
