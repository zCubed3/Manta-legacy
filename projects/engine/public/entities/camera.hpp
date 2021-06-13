#ifndef MANTA_CAMERAENT_H
#define MANTA_CAMERAENT_H

#include <glm/glm/glm.hpp>

class Renderer;

// Each render backend utilizes this differently
class Camera {
   public:
      glm::vec3 position = glm::vec3(0, 0, -5); //Bring the camera back by default
      glm::vec3 angles;

      float fieldOfView = 80;

      float nearClip = 0.001f;
      float farClip = 100.0f;

      glm::mat4 view;
      glm::mat4 perspective;

      void Update(Renderer* renderer);
};

#endif
