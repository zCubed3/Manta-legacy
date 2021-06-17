#include <entities/world.hpp>

#include <stdio.h>

#include <entities/light.hpp>

#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

void World::CreateConObjects(Console* console) {
   if (console) {
      //
      // CFuncs
      //

      //console->Create
      //
      this->console = console;
   }
}

void World::Update() {

   int l = 0;
   for (int e = 0; e < entities.size(); e++) {
      if (entities[e] == nullptr) {
	 printf("Entity #%i is a nullptr\n", e);
	 continue;
      }

      Light* light = dynamic_cast<Light*>(entities[e]);

      entities[e]->Update(this);

      if (light != nullptr) {
	 data.lightPositions[l] = light->position;
	 data.lightDirections[l] = glm::rotate(light->rotation, glm::vec3(0, 0, 1));
	 data.lightColors[l] = light->color;
	 data.lightRanges[l] = light->range;
	 data.lightIntensities[l] = light->intensity;
	 data.lightParams1[l] = light->param1;
	 data.lightParams2[l] = light->param2;
	 data.lightTypes[l] = (int)light->type;
	 l++;	 
      }	 
   }

   data.lightCount = l;
}

void World::Draw(Renderer* renderer) {
   timeTotal = (float)glfwGetTime();
   timeDelta = timeTotal - timeLast;
   timeLast = timeTotal;

   for (int e = 0; e < entities.size(); e++) {
      if (entities[e] == nullptr)
	 continue;

      entities[e]->Draw(renderer);
   }
}
