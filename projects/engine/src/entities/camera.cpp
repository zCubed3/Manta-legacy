#include "glm/glm/ext/matrix_transform.hpp"
#include <entities/camera.hpp>

#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/quaternion.hpp>

#include <renderer.hpp>
#include <console.hpp>
#include <entities/world.hpp>

void Camera::Update(World* world) {
   Entity::Update(world);

   glm::vec3 forward = glm::rotate(rotation, glm::vec3(0, 0, 1));

   view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));

   if (!ignoreConFov && world)
      if (world->console)
	 fieldOfView = world->console->CVarGetInt("fov", 90);

   if (renderer != nullptr)
      perspective = glm::perspective(glm::radians(fieldOfView), renderer->windowWidth / renderer->windowHeight, nearClip, farClip); 
}
