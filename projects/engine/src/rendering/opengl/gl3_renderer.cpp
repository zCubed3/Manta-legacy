#include "gl3_renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <stdio.h>
#include <string.h>
#include <console/console.hpp>

#include "assets/glmodel.hpp"
#include "assets/glshader.hpp"
#include "assets/gltexture.hpp"

#include <entities/light.hpp>
#include <entities/world.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <assets/resources.hpp>

//
// Callbacks
//

// TODO: Make this set a permanent variable so RenderTargets are possible
void OnFramebufferResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    void *rendererRaw = glfwGetWindowUserPointer(window);
    if (!rendererRaw)
        return;

    GL3Renderer *renderer = (GL3Renderer *) rendererRaw;

    if (renderer) {
        renderer->windowWidth = width;
        renderer->windowHeight = height;
        renderer->CreateGBuffers(); // We need to recreate gbuffers upon resizing the window
    }
}

//
// Renderer functionality
//

// Shadowmaps based on https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
void GL3Renderer::Initialize() {
    glewExperimental = true; // Enables more features

    if (glfwInit()) {
        printf("Initialized GLFW\n");
        glfwWindowHint(GLFW_RESIZABLE, console->CVarGetBool("r_window_resizable", false));
        glfwWindowHint(GLFW_SAMPLES, console->CVarGetInt("r_msaa_samples", 1));
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWmonitor *monitor = nullptr;

        if (console->CVarGetBool("fullscreen", false))
            monitor = glfwGetPrimaryMonitor();

        window = glfwCreateWindow(console->CVarGetInt("width", 800), console->CVarGetInt("height", 600),
                                  console->CVarGetData("gamename", "Manta").c_str(), monitor, nullptr);
        glfwMakeContextCurrent(window);

        // Supply the renderer some info
        int intWidth, intHeight;
        glfwGetWindowSize(window, &intWidth, &intHeight);
        windowWidth = intWidth;
        windowHeight = intHeight;

        // Depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        //glEnable(GL_FRAMEBUFFER_SRGB);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

        if (glewInit() != GLEW_OK) {
            printf("Failed to initialze GLEW!\n");
            return;
        }

        // Internal engine shaders
        CreateShaderProgram(resources->shaderLoader.LoadCode("engine#error", gl3ErrorShaderCode));

        // GBuffer Framebuffer
        glGenFramebuffers(1, &gbufferFBO);
        CreateGBuffers();

        // Load default content
        cameraQuad = resources->modelLoader.LoadModel(console->CVarGetData("r_model_quad", ""));
        CreateVertexBuffer(cameraQuad);
        lightingShader = resources->shaderLoader.LoadShader(console->CVarGetData("r_shader_lighting", ""));
        CreateShaderProgram(lightingShader);
    } else {
        printf("Failed to initialize GLFW\n");
    }
}

void GL3Renderer::BeginRender(RenderType renderType) {
    if (renderType == RenderType::GBuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferPositionID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbufferNormalID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbufferAlbedoID, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gbufferEmissionID, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbufferDepthRBO);

        uint buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, buffers);
    }

    if (renderType == RenderType::Shadowmap) {
        glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowmapID, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (vsync)
        glfwSwapInterval(vsync);
}

Renderer::Status GL3Renderer::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Renderer::Status state = Renderer::Status::Running;

    if (glfwWindowShouldClose(window)) {
        state = Renderer::Status::ShuttingDown;
        glfwDestroyWindow(window);
    }

    return state;
}

void GL3Renderer::PresentRender() {
    glfwSwapBuffers(window);
}

void GL3Renderer::InitImGui() {
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void GL3Renderer::BeginImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GL3Renderer::EndImGui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GL3Renderer::CreateVertexBuffer(Model *model) {
    if (model) {
        if (model->vertexBuffer) {
            printf("Model already has a vertex buffer, deleting it!\n");
            delete model->vertexBuffer;
        }

        model->vertexBuffer = new GL3VertexBuffer();
        model->vertexBuffer->Populate(model);
    }
}

void GL3Renderer::CreateShaderProgram(Shader *shader) {
    if (shader) {
        if (shader->program != nullptr) {
            printf("Shader already has a program, deleting it!\n");
            delete shader->program;
        }

        GL3ShaderProgram *program = new GL3ShaderProgram();
        if (program->Compile(shader))
            shader->program = program;
    }
}

void GL3Renderer::CreateTextureBuffer(Texture *texture) {
    if (texture) {
        if (texture->texBuffer) {
            printf("Texture already has a texture buffer, deleting it!\n");
            delete texture->texBuffer;
        }

        texture->texBuffer = new GL3TextureBuffer();
        texture->texBuffer->Populate(texture);
    }
}

void GL3Renderer::CreateConObjects(Console *console) {
    Renderer::CreateConObjects(console);
    // Doesn't do anything yet
}

void GL3Renderer::DrawImGuiWindow() {
    if (!showImGuiWindow)
        return;

    ImGui::Begin("Renderer");

    ImGui::Text("OpenGL 3.3");
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

        ImGui::Text("Emissive:");
        ImGui::Image((void *) (intptr_t) gbufferEmissionID, ImVec2(windowWidth / 4, windowHeight / 4), ImVec2(0, 1),
                     ImVec2(1, 0));

        ImGui::TreePop();
    }

    ImGui::End();
}

void GL3Renderer::CreateGBuffers() {
    std::vector<Texture *> gbuffers{gbufferPositionTex, gbufferNormalTex, gbufferAlbedoTex, gbufferEmissionTex};

    if (!gbufferInited) {
        gbufferPositionTex = resources->textureLoader.CreateTexture("engine#gbuffer#position", windowWidth,
                                                                    windowHeight,
                                                                    Texture::TextureType::Texture2D,
                                                                    Texture::Format::Internal,
                                                                    Texture::Filtering::Point);

        gbufferPositionTex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferNormalTex = resources->textureLoader.CreateTexture("engine#gbuffer#normal", windowWidth, windowHeight,
                                                                  Texture::TextureType::Texture2D,
                                                                  Texture::Format::Internal, Texture::Filtering::Point);

        gbufferNormalTex->dataFormat = Texture::DataFormat::RGBA_16F;

        gbufferAlbedoTex = resources->textureLoader.CreateTexture("engine#gbuffer#albedo", windowWidth, windowHeight,
                                                                  Texture::TextureType::Texture2D,
                                                                  Texture::Format::Internal, Texture::Filtering::Point);

        gbufferEmissionTex = resources->textureLoader.CreateTexture("engine#gbuffer#emission", windowWidth,
                                                                    windowHeight,
                                                                    Texture::TextureType::Texture2D,
                                                                    Texture::Format::Internal,
                                                                    Texture::Filtering::Point);

        glGenRenderbuffers(1, &gbufferDepthRBO);

        gbuffers = {gbufferPositionTex, gbufferNormalTex, gbufferAlbedoTex, gbufferEmissionTex};

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
    gbufferEmissionID = gbufferEmissionTex->texBuffer->handle;
}

void GL3Renderer::DrawLightingQuad() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbufferPositionID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbufferNormalID);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbufferAlbedoID);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gbufferEmissionID);

    cameraQuad->shader = lightingShader;
    cameraQuad->Draw(this, resources, nullptr);
}

//
// Built-in shaders
//

std::string gl3ErrorShaderCode = R"(
#ifdef VERTEX
layout(location = 0) in vec3 _vertex;
layout(location = 1) in vec3 _normal;

uniform mat4 MANTA_mM;
uniform mat4 MANTA_mV;
uniform mat4 MANTA_mP;

out vec3 vert_position;
out vec3 normal;

void main() {
  mat4 mMVP = MANTA_mP * MANTA_mV * MANTA_mM;
  gl_Position = mMVP * vec4(_vertex, 1.0);

  vert_position = (MANTA_mM * vec4(_vertex, 1.0)).xyz;

  mat3 mx3norm = mat3(MANTA_mM);
  mx3norm = inverse(mx3norm);
  mx3norm = transpose(mx3norm);

  normal = (mx3norm * _normal);
}

#endif

#ifdef FRAGMENT
layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 3) out vec4 out_emission;

uniform float MANTA_fTime;

in vec3 vert_position;
in vec3 normal;

void main() {
   out_position = vec4(vert_position, 1);
   out_normal = vec4(normal, 1);

   // Color is #ff7300
   out_emission = vec4(vec3(1.0, 0.45098039215, 0.0) * abs(sin(MANTA_fTime * 2)), 1.0);
}

#endif
)";
