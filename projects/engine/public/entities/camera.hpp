#ifndef MANTA_CAMERAENT_H
#define MANTA_CAMERAENT_H

#include <glm/glm/glm.hpp>

#include <entities/entity.hpp>

class Renderer;

class Camera : public Entity {
   public:
      float fieldOfView = 80;

      float nearClip = 0.001f;
      float farClip = 100.0f;

      glm::mat4 view;
      glm::mat4 perspective;

      Renderer* renderer;

      virtual void Update() override;
};

#endif
