#ifndef MANTA_SKYBOX_HPP
#define MANTA_SKYBOX_HPP

#include <actors/actor.hpp>

class ACamera; // Skyboxes need a camera to work

class Cubemap;

class ASkybox : public AActor {
public:

    ACamera *pCamera = nullptr;
    Cubemap *pCubemap = nullptr;

    void Start(MEngine *engine) override;

    void Update(MEngine *engine) override;

    void Draw(MEngine *engine) override;
};


#endif
