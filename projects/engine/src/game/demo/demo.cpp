#include "demo.hpp"

#include <core/engine.hpp>

#include <GLFW/glfw3.h>

#include <components/engine/renderer.hpp>

#include <assets/material.hpp>
#include <assets/material_values/material_float.hpp>
#include <assets/material_values/material_color.hpp>

// Use this for creating things within the world upon loading the game
void MDemoGame::Start(MEngine *engine) {
    glfwGetCursorPos(engine->renderer->window, &mouseX, &mouseY);

#ifdef PROBE_TESTER
#define PROBE_COUNT_X 10
#define PROBE_COUNT_Y 10
#define PROBE_DISTANCE 0.275f
#define PROBE_SIZE 0.125f
#define PROBE_RGB 1.0f, 0.5f, 0.0f
//#define PROBE_RGB 1.0f, 1.0f, 1.0f
//#define PROBE_ORBIT_LIGHTS

    for (int x = 0; x < PROBE_COUNT_X; x++) {
        for (int y = 0; y < PROBE_COUNT_Y; y++) {
            AActor *actor = new AActor();

            float actual_x = x - (PROBE_COUNT_X / 2);
            float actual_y = y - (PROBE_COUNT_Y / 2);

            actor->position = glm::vec3(actual_x * PROBE_DISTANCE, actual_y * PROBE_DISTANCE, 0);
            actor->scale = glm::vec3(PROBE_SIZE, PROBE_SIZE, PROBE_SIZE);

            actor->name = "TestActor";

            CRenderer *renderer = new CRenderer();
            actor->AddComponent(renderer);

            char testMatName[24];
            sprintf(testMatName, "TestMat_%i_%i", x, y);

            auto *mat = engine->resources.CreateMaterial(engine, std::string(testMatName), "engine#shader#standard");

            ((MaterialFloat *) mat->values[0])->value = x / (float) PROBE_COUNT_X;
            ((MaterialFloat *) mat->values[1])->value = y / (float) PROBE_COUNT_Y;
            ((MaterialColor *) mat->values[2])->color = glm::vec4(PROBE_RGB, 1.0);

            renderer->AddModel(engine->resources.modelLoader.loadedModels["engine#sphere"], mat);

            engine->world.AddActor(engine, actor);
        }
    }

#ifdef PROBE_ORBIT_LIGHTS
    rLight = new ALight();
    rLight->color = glm::vec3(1, 0, 0);
    engine->world.AddActor(engine, rLight);

    gLight = new ALight();
    gLight->color = glm::vec3(0, 1, 0);
    engine->world.AddActor(engine, gLight);

    bLight = new ALight();
    bLight->color = glm::vec3(0, 0, 1);
    engine->world.AddActor(engine, bLight);

    rLight->type = gLight->type = bLight->type = ALight::LightType::Point;
#else
    ALight *light = new ALight();
    engine->world.AddActor(engine, light);
#endif
#endif
}

void MDemoGame::Update(MEngine *engine) {
    double nowMouseX, nowMouseY;
    glfwGetCursorPos(engine->renderer->window, &nowMouseX, &nowMouseY);

    //if (lockCursor) {
    ACamera *camera = engine->world.pCamera;
    Renderer *renderer = engine->renderer;
    World *world = &engine->world;

    if (lockCursor) {
        camera->euler += glm::vec3(nowMouseY - mouseY, mouseX - nowMouseX, 0) * 0.1f;

        if (glfwGetKey(renderer->window, GLFW_KEY_W))
            camera->position += glm::rotate(camera->rotation, glm::vec3(0, 0, 1)) * world->timeDelta;

        if (glfwGetKey(renderer->window, GLFW_KEY_S))
            camera->position += glm::rotate(camera->rotation, glm::vec3(0, 0, -1)) * world->timeDelta;

        if (glfwGetKey(renderer->window, GLFW_KEY_A))
            camera->position += glm::rotate(camera->rotation, glm::vec3(1, 0, 0)) * world->timeDelta;

        if (glfwGetKey(renderer->window, GLFW_KEY_D))
            camera->position += glm::rotate(camera->rotation, glm::vec3(-1, 0, 0)) * world->timeDelta;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !hasLocked) {
        lockCursor = !lockCursor;
        glfwSetInputMode(renderer->window, GLFW_CURSOR, lockCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        hasLocked = true;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        hasLocked = false;

    mouseX = nowMouseX;
    mouseY = nowMouseY;

#ifdef PROBE_ORBIT_LIGHTS
    float pi = 3.1415926535f;
    float piThird = 2.094400006763f;

    rLight->position = glm::vec3(sinf(engine->world.timeTotal + piThird * 1) * 3,
                                 cosf(engine->world.timeTotal + piThird * 1) * 3,
                                 0);

    gLight->position = glm::vec3(sinf(engine->world.timeTotal + piThird * 2) * 3,
                                 cosf(engine->world.timeTotal + piThird * 2) * 3,
                                 0);

    bLight->position = glm::vec3(sinf(engine->world.timeTotal + piThird * 3) * 3,
                                 cosf(engine->world.timeTotal + piThird * 3) * 3,
                                 0);
#endif

    engine->world.Update(engine);
    engine->hideUI = lockCursor;
}

void MDemoGame::Draw(MEngine *engine) {

}