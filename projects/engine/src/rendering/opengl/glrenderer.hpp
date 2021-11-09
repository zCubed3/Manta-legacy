#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <rendering/renderer.hpp>

typedef unsigned int uint;

typedef struct GLFWwindow GLFWwindow;

class Texture;

#define MANTA_RENDERER_TYPE GLRenderer

class GLRenderer : public Renderer {
public:
    virtual const char *get_APIName() override { return "OpenGL 4.6"; };

    void Initialize(MEngine *engine) override;

    void BeginRender(MEngine *engine, RenderType renderType) override;

    Status EndRender() override;

    void PresentRender() override;

    void CreateVertexBuffer(Model *model) override;

    void CreateShaderProgram(Shader *shader) override;

    void CreateTextureBuffer(Texture *texture) override;

    void CreateCubemapBuffer(Cubemap *cubemap) override;

    virtual void CreateConObjects(Console *console) override;

    void InitImGui() override;

    void BeginImGui() override;

    void EndImGui() override;

    void DrawImGuiWindow() override;

    bool gbufferInited = false;
    uint gbufferFBO, gbufferDepthRBO;
    Texture *gbufferPositionTex, *gbufferNormalTex, *gbufferAlbedoTex, *gbufferMRATex, *gbufferEmissionTex;
    uint gbufferPositionID, gbufferNormalID, gbufferAlbedoID, gbufferMRAID, gbufferEmissionID;

    uint shadowmapFBO, shadowmapID;

    bool gbufferDirty = false;

    void CreateGBuffers(MEngine *engine);

    void DrawLightingQuad(MEngine *engine) override;

    void SetCullingMode(CullMode mode) override;

    void SetDepthTestMode(DepthMode mode) override;
};

extern std::string gl3ErrorShaderCode; // Fallback shader that is used in place of a nullptr

#endif
