#include "engine.hpp"

#include <game/game.hpp>

#include <manta.hpp>

#include <console/common_console.hpp>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

MEngine::MEngine(int argc, char** argv) {
    CreateCommonConObjects(&console);
    world.CreateConObjects(&console);

    renderer = new MANTA_RENDERER_TYPE();
    renderer->CreateConObjects(&console);

    console.ParseExecFile("./autoexec"); // Autoexec is the default autoexec path
    console.ParseCommandLine(argc, argv);

    renderer->Initialize(this);

    ImGui::CreateContext();
    renderer->InitImGui();
    ImGuiStyle &style = ImGui::GetStyle();

    ImGui::StyleColorsDark(&style);

    ImGuiIO &imguiIO = ImGui::GetIO();
    imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    imguiIO.Fonts->AddFontFromFileTTF("engine/base/fonts/Roboto-Regular.ttf", 14);

    resources.LoadBaseContent(this);

    world.Initialize(this);

    game = new MANTA_GAME_TYPE();
    game->Start(this);
}

void MEngine::EngineLoop() {
    while (alive) {
        // GLFW callbacks are the very first thing we check
        glfwPollEvents();

        // TODO: EditorLoop()
        game->Update(this);

        // Render the gbuffers
        renderer->BeginRender(this, Renderer::RenderType::GBuffer);

        world.Draw(this);

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }

        // Render the lighting pass
        renderer->BeginRender(this, Renderer::RenderType::Default);
        renderer->DrawLightingQuad(this);

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }

        // Then the UI
        renderer->BeginImGui();

        if (!hideUI) {
            ImGui::Begin("Windows");

            ImGui::Checkbox("Renderer", &renderer->showImGuiWindow);
            ImGui::Checkbox("World", &world.showWindow);
            ImGui::Checkbox("Resources", &resources.showWindow);
            ImGui::Checkbox("Console", &console.showWindow);

            ImGui::End();

            world.DrawImGuiWindow(&resources);
            renderer->DrawImGuiWindow();
            resources.DrawImGuiWindow(this);
            console.DrawImGuiWindow();
        }

        renderer->EndImGui();

        renderer->PresentRender();
    }
}