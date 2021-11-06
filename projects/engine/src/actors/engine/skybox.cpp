#include "skybox.hpp"

#include <components/engine/renderer.hpp>

#include <actors/world.hpp>
#include <actors/engine/camera.hpp>

#include <assets/resources.hpp>
#include <assets/cubemap.hpp>

#include <rendering/renderer.hpp>

#include <GL/glew.h>

#include <core/engine.hpp>

void ASkybox::Start(MEngine *engine) {
    pCubemap = engine->resources.cubemapLoader.cubemaps["engine#default_cubemap"];
}

void ASkybox::Update(MEngine *engine) {
    if (pCamera != nullptr) {
        position = pCamera->position;
    }

    AActor::Update(engine);
}

void ASkybox::Draw(MEngine *engine) {
    // We need to change two things within the renderer for the skybox to work
    // Disable culling and depth testing
    engine->renderer->SetCullingMode(Renderer::CullMode::None);
    engine->renderer->SetDepthTestMode(Renderer::DepthMode::None);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, pCubemap->buffer->handle);

    engine->resources.modelLoader.loadedModels["engine#cube"]->Draw(engine, this,
                                                                    engine->resources.materialLoader.materials["engine#skybox"]);

    // Restore old rendering
    engine->renderer->SetCullingMode(Renderer::CullMode::Back);
    engine->renderer->SetDepthTestMode(Renderer::DepthMode::Less);
}