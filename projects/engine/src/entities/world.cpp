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
	 //data.lightPositions[l] = glm::value_ptr(light->position);
	 l++;	 
      }	 

      entities[e]->Update();
   }

   data.lightCount = l;
}
