#ifndef MANTA_LIGHT_H
#define MANTA_LIGHT_H

#include "actors/actor.hpp"

class Texture;

class ALight : public AActor {
public:
    ALight();

    void CreateShadowmap();

    glm::vec3 color = glm::vec3(1, 1, 1);
    float range = 10.0f;
    float intensity = 1.0f;

    // Used for certain lighting types
    // Spot: 1 = inner angle, 2 = outer angle
    float param1 = 1.0f;
    float param2 = 10.0f;

    enum class LightType {
        Sun,
        Point,
        Spot
    };

    int shadowmapWidth = 256;
    int shadowmapHeight = 256;

    LightType type = LightType::Sun;

    Texture *shadowmap;

    glm::mat4 mLightMatrix; // Lights are practically cameras
    virtual void Update(MEngine *engine) override;

    virtual void DrawImGuiSub(World *world, int index) override;
};

#endif
