#include "glrenderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <console/console.hpp>

#include "assets/glmodel.hpp"
#include "assets/glshader.hpp"
#include "assets/gltexture.hpp"
#include "assets/glcubemap.hpp"

#include <actors/engine/light.hpp>
#include <actors/world.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <assets/resources.hpp>
#include <assets/material.hpp>

#include <core/engine.hpp>

//
// Callbacks
//

// TODO: Make this set a permanent variable so RenderTargets are possible
void OnFramebufferResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    void *rendererRaw = glfwGetWindowUserPointer(window);
    if (!rendererRaw)
        return;

    GLRenderer *renderer = (GLRenderer *) rendererRaw;

    if (renderer) {
        renderer->windowWidth = width;
        renderer->windowHeight = height;
        renderer->gbufferDirty = true; // We queue up the renderer to recreate gbuffers upon resizing the window
    }
}

//
// Renderer functionality
//
// Shadowmaps based on https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping

void GLRenderer::Initialize(MEngine *engine) {
    glewExperimental = true; // Enables more features

    if (glfwInit()) {
        printf("Initialized GLFW\n");
        glfwWindowHint(GLFW_RESIZABLE, engine->console.CVarGetBool("r_window_resizable", false));
        glfwWindowHint(GLFW_SAMPLES, engine->console.CVarGetInt("r_msaa_samples", 1));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

        GLFWmonitor *monitor = nullptr;

        if (engine->console.CVarGetBool("fullscreen", false))
            monitor = glfwGetPrimaryMonitor();

        window = glfwCreateWindow(engine->console.CVarGetInt("width", 800), engine->console.CVarGetInt("height", 600),
                                  engine->console.CVarGetData("gamename", "Manta").c_str(), monitor, nullptr);

        glfwMakeContextCurrent(window);

        // Supply the renderer some info
        int intWidth, intHeight;
        glfwGetWindowSize(window, &intWidth, &intHeight);
        windowWidth = (float) intWidth;
        windowHeight = (float) intHeight;

        // Depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_DITHER);

        //glEnable(GL_FRAMEBUFFER_SRGB);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

        if (glewInit() != GLEW_OK) {
            printf("Failed to initialze GLEW!\n");
            return;
        }

        // Internal engine materials and shaders
        auto errorShader = engine->resources.shaderLoader.LoadCode("engine#error", gl3ErrorShaderCode);
        CreateShaderProgram(errorShader);
        Material::errorMaterial = engine->resources.materialLoader.CreateMaterial(&engine->resources, "engine#error", errorShader,
                                                                                  false); // We disable defaults since this isn't a normal material

        // GBuffer Framebuffer
        glGenFramebuffers(1, &gbufferFBO);
        CreateGBuffers(engine);
    } else {
        printf("Failed to initialize GLFW\n");
    }
}

void GLRenderer::BeginRender(MEngine *engine, RenderType renderType) {
    if (renderType == RenderType::GBuffer) {
        if (gbufferDirty)
            CreateGBuffers(engine);

        glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferPositionID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbufferNormalID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbufferAlbedoID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gbufferMRAID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gbufferEmissionID, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbufferDepthRBO);

        uint buffers[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                           GL_COLOR_ATTACHMENT4};
        glDrawBuffers(5, buffers);
    }

    if (renderType == RenderType::Shadowmap) {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowmapID, 0);

        glDrawBuffer(GL_DEPTH_ATTACHMENT);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapInterval(0);
}

Renderer::Status GLRenderer::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Renderer::Status state = Renderer::Status::Running;

    if (glfwWindowShouldClose(window)) {
        state = Renderer::Status::ShuttingDown;
        glfwDestroyWindow(window);
    }

    return state;
}

void GLRenderer::PresentRender() {
    glfwSwapBuffers(window);
}

void GLRenderer::InitImGui() {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void GLRenderer::BeginImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GLRenderer::EndImGui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLRenderer::CreateVertexBuffer(Model *model) {
    if (model) {
        if (model->vertexBuffer) {
            printf("Model already has a vertex buffer, deleting it!\n");
            delete model->vertexBuffer;
        }

        model->vertexBuffer = new GLVertexBuffer();
        model->vertexBuffer->Populate(model);
    }
}

void GLRenderer::CreateShaderProgram(Shader *shader) {
    if (shader) {
        if (shader->program != nullptr) {
            printf("Shader already has a program, deleting it!\n");
            delete shader->program;
        }

        auto *program = new GLShaderProgram();
        if (program->Compile(shader))
            shader->program = program;
    }
}

void GLRenderer::CreateTextureBuffer(Texture *texture) {
    if (texture) {
        if (texture->texBuffer) {
            printf("Texture already has a texture buffer, deleting it!\n");
            delete texture->texBuffer;
        }

        texture->texBuffer = new GLTextureBuffer();
        texture->texBuffer->Populate(texture);
    }
}

void GLRenderer::CreateCubemapBuffer(Cubemap *cubemap) {
    if (cubemap) {
        if (cubemap->buffer) {
            printf("Cubemap already has a texture buffer, deleting it!\n");
            delete cubemap->buffer;
        }

        cubemap->buffer = new GLCubemapBuffer();
        cubemap->buffer->Populate(cubemap);
    }
}

void GLRenderer::CreateConObjects(Console *console) {
    Renderer::CreateConObjects(console);
    // Doesn't do anything yet
}

void GLRenderer::DrawImGuiWindow() {
    if (!showImGuiWindow)
        return;

    ImGui::Begin("Renderer");

    ImGui::Text("%s", get_APIName());
    ImGui::Checkbox("VSync", &vsync);

    if (ImGui::TreeNode("GBuffer Visualization##gbuf_vis_opengl")) {
        ImGui::Text("Positions:");
        ImGui::Image((void *) (intptr_t) gbufferPositionID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));
        ImGui::Spacing();

        ImGui::Text("Normals:");
        ImGui::Image((void *) (intptr_t) gbufferNormalID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));
        ImGui::Spacing();

        ImGui::Text("Albedo:");
        ImGui::Image((void *) (intptr_t) gbufferAlbedoID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));
        ImGui::Spacing();

        ImGui::Text("MRA:");
        ImGui::Image((void *) (intptr_t) gbufferMRAID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));

        ImGui::Text("Emissive:");
        ImGui::Image((void *) (intptr_t) gbufferEmissionID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));

        ImGui::TreePop();
    }

    ImGui::End();
}

void GLRenderer::CreateGBuffers(MEngine *engine) {
    gbufferDirty = false;

    std::vector<Texture *> gbuffers{gbufferPositionTex, gbufferNormalTex, gbufferAlbedoTex, gbufferMRATex,
                                    gbufferEmissionTex};

    if (!gbufferInited) {
        gbufferPositionTex = engine->resources.textureLoader.CreateTexture("engine#gbuffer#position", windowWidth,
                                                                           windowHeight,
                                                                           Texture::Format::Internal,
                                                                           Texture::Filtering::Point);

        gbufferPositionTex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferNormalTex = engine->resources.textureLoader.CreateTexture("engine#gbuffer#normal", windowWidth,
                                                                         windowHeight,
                                                                         Texture::Format::Internal,
                                                                         Texture::Filtering::Point);

        gbufferNormalTex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferAlbedoTex = engine->resources.textureLoader.CreateTexture("engine#gbuffer#albedo", windowWidth,
                                                                         windowHeight,
                                                                         Texture::Format::Internal,
                                                                         Texture::Filtering::Point);

        gbufferAlbedoTex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferMRATex = engine->resources.textureLoader.CreateTexture("engine#gbuffer#mrs", windowWidth,
                                                                      windowHeight,
                                                                      Texture::Format::Internal,
                                                                      Texture::Filtering::Point);

        gbufferMRATex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferEmissionTex = engine->resources.textureLoader.CreateTexture("engine#gbuffer#emission", windowWidth,
                                                                           windowHeight,
                                                                           Texture::Format::Internal,
                                                                           Texture::Filtering::Point);

        glGenRenderbuffers(1, &gbufferDepthRBO);

        gbuffers = {gbufferPositionTex, gbufferNormalTex, gbufferAlbedoTex, gbufferMRATex, gbufferEmissionTex};

        gbufferInited = true;
    } else {
        for (auto &gbuffer: gbuffers)
            gbuffer->Resize(windowWidth, windowHeight);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, gbufferDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, windowWidth, windowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    for (auto &gbuffer: gbuffers)
        CreateTextureBuffer(gbuffer);

    gbufferPositionID = gbufferPositionTex->texBuffer->handle;
    gbufferNormalID = gbufferNormalTex->texBuffer->handle;
    gbufferAlbedoID = gbufferAlbedoTex->texBuffer->handle;
    gbufferMRAID = gbufferMRATex->texBuffer->handle;
    gbufferEmissionID = gbufferEmissionTex->texBuffer->handle;
}

void GLRenderer::DrawLightingQuad(MEngine *engine) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbufferPositionID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbufferNormalID);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbufferAlbedoID);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gbufferMRAID);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gbufferEmissionID);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, engine->resources.textureLoader.loadedTextures["engine#brdf_lut"]->texBuffer->handle);

    SetCullingMode(CullMode::None);
    auto lighting = engine->resources.materialLoader.materials["engine#deferred_lighting"];
    engine->resources.modelLoader.loadedModels["engine#quad"]->Draw(engine, nullptr, lighting);
}

void GLRenderer::SetCullingMode(CullMode mode) {
    switch (mode) {
        case CullMode::None:
            glDisable(GL_CULL_FACE);
            return;

        case CullMode::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            return;

        case CullMode::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            return;
    }
}

void GLRenderer::SetDepthTestMode(DepthMode mode) {
    switch (mode) {
        case DepthMode::None:
            glDisable(GL_DEPTH_TEST);
            return;

        case DepthMode::Less:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            return;

        case DepthMode::Greater:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GREATER);
            return;
    }
}

//
// Built-in shaders
//

std::string gl3ErrorShaderCode = R"(#version 330 core

#ifdef VERTEX
layout(location = 0) in vec3 _vertex;
layout(location = 1) in vec3 _normal;

uniform mat4 MANTA_MATRIX_M;
uniform mat4 MANTA_MATRIX_V;
uniform mat4 MANTA_MATRIX_P;
uniform mat4 MANTA_MATRIX_M_IT;

out vec3 vert_position;
out vec3 normal;

void main() {
  mat4 mMVP = MANTA_MATRIX_P * MANTA_MATRIX_V * MANTA_MATRIX_M;
  gl_Position = mMVP * vec4(_vertex, 1.0);

  vert_position = (MANTA_MATRIX_M * vec4(_vertex, 1.0)).xyz;

  normal = (MANTA_MATRIX_M * vec4(_normal, 1)).xyz;
}

#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_color;
layout(location = 3) out vec4 out_mrs;
layout(location = 4) out vec4 out_emission;

uniform float MANTA_TIME;

in vec3 vert_position;
in vec3 normal;

void main() {
    out_position = vec4(vert_position, 1);
    out_normal = vec4(normal, 1);

    out_color = vec4(0, 0, 0, 1);
    // Color is #ff7300
    out_emission = vec4(vec3(1.0, 0.45098039215, 0.0) * abs(sin(MANTA_TIME * 2)), 1.0);
    out_mrs = vec4(0.0, 1.0, 0.0, 1.0);
}

#endif
)";
