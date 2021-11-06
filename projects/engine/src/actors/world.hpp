#ifndef MANTA_WORLD_H
#define MANTA_WORLD_H

#include <vector>
#include <glm/glm/glm.hpp>

class AActor;

class ALight;

class Console;

class Renderer;

class Resources;

#include <actors/engine/light.hpp>

#define MAX_LIGHTS 32

class WorldData {
public:
    glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);

    int lightCount = 0;
    glm::vec3 lightPositions[MAX_LIGHTS];
    glm::vec3 lightDirections[MAX_LIGHTS];
    glm::vec3 lightColors[MAX_LIGHTS];
    float lightRanges[MAX_LIGHTS];
    float lightIntensities[MAX_LIGHTS];
    float lightParams1[MAX_LIGHTS];
    float lightParams2[MAX_LIGHTS];
    int lightTypes[MAX_LIGHTS];

    ALight *lights[MAX_LIGHTS];
};

class ACamera;

class ASkybox;

class MEngine;

class World {
public:
    std::vector<AActor *> actors;

    WorldData data;
    Console *console;

    ACamera *pCamera = nullptr;
    ASkybox *pSkybox = nullptr;

    void Initialize(MEngine *engine);

    void CreateConObjects(Console *console);

    void AddActor(MEngine *engine, AActor *actor);

    void Update(MEngine *engine);

    void Draw(MEngine *engine);

    void CustomRender(MEngine *engine);

    float timeTotal = 0.0f;
    float timeLast = 0.0f;
    float timeDelta = 0.0f;

    bool showWindow = false;

    void DrawImGuiWindow(Resources *resources);

    void CFunc_CreateEntity(Console *console, std::vector<std::string> args);
};

#endif
