#ifndef MANTA_CAMERAENT_H
#define MANTA_CAMERAENT_H

#include "actors/actor.hpp"

class Renderer;

class ACamera : public AActor {
public:
    float fieldOfView = 80;

    float nearClip = 0.001f;
    float farClip = 100.0f;

    glm::mat4 mView;
    glm::mat4 mPerspective;

    bool ignoreConFov = true;

    virtual void Update(MEngine *engine) override;

    virtual void DrawImGuiSub(World *world, int index) override;
};

#endif
