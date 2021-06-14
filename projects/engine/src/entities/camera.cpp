#include "glm/glm/ext/matrix_transform.hpp"
#include <entities/camera.hpp>

#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/quaternion.hpp>

#include <renderer.hpp>

void Camera::Update() {
   glm::quat rot = glm::quat(euler);
   glm::vec3 forward = glm::vec3(0, 0, 1) * rot;

   view = glm::lookAt(position, glm::vec3(0,0,0), glm::vec3(0, 1, 0));

   if (renderer != nullptr)
      perspective = glm::perspective(glm::radians(fieldOfView), renderer->windowWidth / renderer->windowHeight, nearClip, farClip); 

   position = glm::vec3(sinf(renderer->timeTotal), cosf(renderer->timeTotal), -3);
}
