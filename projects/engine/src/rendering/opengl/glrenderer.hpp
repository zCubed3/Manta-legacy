#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <rendering/renderer.hpp>

typedef unsigned int uint;

typedef struct GLFWwindow GLFWwindow;

class Texture;

class GLRenderer : public Renderer {
public:
    virtual const char *get_APIName() override { return "OpenGL 4.6"; };

    void Initialize() override;

    void BeginRender(RenderType renderType) override;

    Status EndRender() override;

    void PresentRender() override;

    void CreateVertexBuffer(Model *model) override;

    void CreateShaderProgram(Shader *shader) override;

    void CreateTextureBuffer(Texture *texture) override;

    virtual void CreateConObjects(Console *console) override;

    void InitImGui() override;

    void BeginImGui() override;

    void EndImGui() override;

    void DrawImGuiWindow() override;

    bool gbufferInited = false;
    uint gbufferFBO, gbufferDepthRBO;
    Texture *gbufferPositionTex, *gbufferNormalTex, *gbufferAlbedoTex, *gbufferMRSTex, *gbufferEmissionTex;
    uint gbufferPositionID, gbufferNormalID, gbufferAlbedoID, gbufferMRSID, gbufferEmissionID;

    uint shadowmapFBO, shadowmapID;

    void CreateGBuffers();

    void DrawLightingQuad() override;
    void SetCullingMode(CullMode mode) override;
    void SetDepthTestMode(DepthMode mode) override;
};

extern std::string gl3ErrorShaderCode; // Fallback shader that is used in place of a nullptr

#endif
