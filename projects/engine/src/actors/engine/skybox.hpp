#ifndef MANTA_SKYBOX_HPP
#define MANTA_SKYBOX_HPP

#include <actors/actor.hpp>

class ACamera; // Skyboxes need a camera to work

class ASkybox : public AActor {
public:
    ACamera* pCamera;

    virtual void Update(World *world) override;
    virtual void Draw(Renderer *renderer, Resources *resources) override;
};


#endif
