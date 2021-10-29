#include "actors/actor.hpp"
#include <rendering/renderer.hpp>

#include <manta_macros.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <deque>

#include <assets/manta_packer.hpp>
#include <assets/model.hpp>
#include <assets/shader.hpp>
#include <assets/resources.hpp>

#include <console/console.hpp>
#include <console/common_console.hpp>

#include <actors/world.hpp>
#include <actors/engine/light.hpp>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <glm/glm/gtc/quaternion.hpp>

#include <rendering/opengl/glrenderer.hpp>

#include <actors/engine/skybox.hpp>

#include <assets/cubemap.hpp>

#include <rendering/opengl/assets/glcubemap.hpp>

#include <core/engine.hpp>

int main(int argc, char **argv) {
#ifdef DEBUG
    printf("[Debug]: This is a debug build of Manta, things will be slower seeing as compilation optimization is disabled plus debug info will be present!\n");
    printf("[Debug]: Be warned, some additional behaviour may be present in a debug build that may or may not work correctly!\n");
#endif

    // Here's the concept behind MEngine
    // Create it and call EngineLoop(), then forget about it!
    MEngine engine;
    engine.EngineLoop();

    Console console;
    Resources resources;
    World world;

    world.resources = &resources;
    resources.world = &world; // Circular, I know

    CreateCommonConObjects(&console);
    world.CreateConObjects(&console);
    //PackerCreateConObjects(&console);

    Renderer *renderer = new GLRenderer();

    if (renderer == nullptr) {
        printf("Fatal: Failed to create renderer\n");
        exit(0);
    }

    renderer->resources = &resources;
    renderer->CreateConObjects(&console);

    resources.renderer = renderer;

    console.ParseExecFile("./autoexec"); // Autoexec is the default autoexec path
    console.ParseCommandLine(argc, argv);

    console.CVarGetData("gamename", "Test");

    renderer->world = &world;
    renderer->Initialize();
    renderer->camera = world.pCamera;

    world.renderer = renderer;

    resources.Prewarm();

    // Skybox needs to be set to something
    world.pSkybox->pCubemap = resources.cubemapLoader.cubemaps["engine#default_cubemap"];

    ALight testLight1, testLight2, testLight3;
    std::vector<ALight *> testLights = {&testLight1, &testLight2, &testLight3};

    for (int l = 0; l < testLights.size(); l++) {
        testLights[l]->type = ALight::LightType::Point;
        world.actors.emplace_back(testLights[l]);

        testLights[l]->scale = glm::vec3(1, 1, 1) * 0.1f;
        testLights[l]->name = "Orbit Light";
    }

    testLight1.color = glm::vec3(1, 0, 0);
    testLight2.color = glm::vec3(0, 1, 0);
    testLight3.color = glm::vec3(0, 0, 1);

    ALight cameraSpot;
    cameraSpot.color = glm::vec3(1, 1, 1);
    cameraSpot.type = ALight::LightType::Spot;
    cameraSpot.range = 5.0f;

    cameraSpot.param1 = 10.0f;
    cameraSpot.param2 = 50.0f;

    cameraSpot.position = glm::vec3(0, 3, 0);
    cameraSpot.euler = glm::vec3(90, 0, 0);

    //world.actors.emplace_back(&cameraSpot);

    Renderer::Status state;

    float pi = 3.1415926535f;
    float piThird = 2.094400006763f;

    double mouseX, mouseY;
    glfwGetCursorPos(renderer->window, &mouseX, &mouseY);

    bool lockCursor = false, hasLocked = false;
    bool forward = false, backward = false;

    ImGui::CreateContext();
    renderer->InitImGui();
    ImGuiStyle &style = ImGui::GetStyle();

    ImGui::StyleColorsDark(&style);

    ImGuiIO &imguiIO = ImGui::GetIO();
    imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    imguiIO.Fonts->AddFontFromFileTTF("engine/base/fonts/Roboto-Regular.ttf", 14);

    auto camera = world.pCamera;

    while (true) {
        glfwPollEvents();

        for (int l = 0; l < testLights.size(); l++) {
            int o = l + 1;
            testLights[l]->position = glm::vec3(sinf(world.timeTotal + piThird * o) * 3,
                                                cosf(world.timeTotal + piThird * o) * 3, sinf(world.timeTotal));
            testLights[l]->euler += glm::vec3(0, 1, 0);
        }

        //RGB light
        //testLight1.color = glm::vec3(abs(sinf(renderer->timeTotal)), abs(sinf(renderer->timeTotal + 1)), abs(sinf(renderer->timeTotal + 2)));

        //
        // Very shitty free camera code :tm:
        //
        double nowMouseX, nowMouseY;
        glfwGetCursorPos(renderer->window, &nowMouseX, &nowMouseY);

        if (lockCursor) {
            camera->euler += glm::vec3(nowMouseY - mouseY, mouseX - nowMouseX, 0) * 0.1f;

            if (glfwGetKey(renderer->window, GLFW_KEY_W))
                camera->position += glm::rotate(camera->rotation, glm::vec3(0, 0, 1)) * world.timeDelta;

            if (glfwGetKey(renderer->window, GLFW_KEY_S))
                camera->position += glm::rotate(camera->rotation, glm::vec3(0, 0, -1)) * world.timeDelta;

            if (glfwGetKey(renderer->window, GLFW_KEY_A))
                camera->position += glm::rotate(camera->rotation, glm::vec3(1, 0, 0)) * world.timeDelta;

            if (glfwGetKey(renderer->window, GLFW_KEY_D))
                camera->position += glm::rotate(camera->rotation, glm::vec3(-1, 0, 0)) * world.timeDelta;
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

        cameraSpot.position = camera->position;
        cameraSpot.euler = camera->euler;

        //testEnt.euler += glm::vec3(1, 0, 0);

        //printf("\rRunning %c", spinner.character);
        //spinner.Spin();

        // Render GBuffers
        renderer->BeginRender(Renderer::RenderType::GBuffer);

        world.Draw(renderer);

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }

        // Then render the default scene
        renderer->BeginRender(Renderer::RenderType::Default);
        renderer->DrawLightingQuad();

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }

        if (!lockCursor) {
            renderer->BeginImGui();

            ImGui::Begin("Windows");

            ImGui::Checkbox("Renderer", &renderer->showImGuiWindow);
            ImGui::Checkbox("World", &world.showWindow);
            ImGui::Checkbox("Resources", &resources.showWindow);
            ImGui::Checkbox("Console", &console.showWindow);

            ImGui::End();

            world.DrawImGuiWindow(&resources);
            renderer->DrawImGuiWindow();
            resources.DrawImGuiWindow();
            console.DrawImGuiWindow();

            renderer->EndImGui();

            if (imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        renderer->PresentRender();
    }
}

