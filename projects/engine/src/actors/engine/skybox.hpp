#ifndef MANTA_SKYBOX_HPP
#define MANTA_SKYBOX_HPP

#include <actors/actor.hpp>

class ACamera; // Skyboxes need a camera to work

class Cubemap;

class ASkybox : public AActor {
public:
    ACamera *pCamera;
    Cubemap *pCubemap;

    virtual void Update(MEngine *engine) override;

    virtual void Draw(Renderer *renderer, Resources *resources) override;
};


#endif
