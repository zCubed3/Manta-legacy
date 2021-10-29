#include "engine.hpp"

#include <game/game.hpp>

#include <manta.hpp>

MEngine::MEngine() {
    renderer = new MANTA_RENDERER_TYPE();
    game = new MANTA_GAME_TYPE();

    resources
}

void MEngine::EngineLoop() {
    while (alive) {
        // TODO: EditorLoop()
        game->Update(this);

        // Render the gbuffers
        renderer->BeginRender(Renderer::RenderType::GBuffer);

        world.Draw(renderer);

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }

        // Render the lighting pass
        renderer->BeginRender(Renderer::RenderType::Default);
        renderer->DrawLightingQuad();

        if (renderer->EndRender() != Renderer::Status::Running) {
            break;
        }
    }
}