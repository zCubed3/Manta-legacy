#ifndef MANTA_CAMERAENT_H
#define MANTA_CAMERAENT_H

#include "entity.hpp"

class Renderer;

class Camera : public Entity {
public:
    float fieldOfView = 80;

    float nearClip = 0.001f;
    float farClip = 100.0f;

    glm::mat4 mView;
    glm::mat4 mPerspective;

    bool ignoreConFov = true;

    Renderer *renderer;

    virtual void Update(World *world) override;

    virtual void DrawImGuiSub(World *world, int index) override;
};

#endif
