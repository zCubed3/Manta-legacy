#include "skybox.hpp"

#include <components/engine/renderer.hpp>

#include <actors/world.hpp>
#include <actors/engine/camera.hpp>

#include <assets/resources.hpp>
#include <assets/cubemap.hpp>

#include <rendering/renderer.hpp>

#include <GL/glew.h>

void ASkybox::Update(World *world) {
    if (pCamera != nullptr) {
        position = pCamera->position;
    }

    AActor::Update(world);
}

void ASkybox::Draw(Renderer *renderer, Resources *resources) {
    // We need to change two things within the renderer for the skybox to work
    // Disable culling and depth testing
    renderer->SetCullingMode(Renderer::CullMode::None);
    renderer->SetDepthTestMode(Renderer::DepthMode::None);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, pCubemap->buffer->handle);

    resources->modelLoader.loadedModels["engine#cube"]->Draw(renderer, resources, this,
                                                             resources->materialLoader.materials["engine#skybox"]);

    // Restore old rendering
    renderer->SetCullingMode(Renderer::CullMode::Back);
    renderer->SetDepthTestMode(Renderer::DepthMode::Less);
}