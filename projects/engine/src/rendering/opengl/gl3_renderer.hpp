#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <rendering/renderer.hpp>

typedef unsigned int uint;

typedef struct GLFWwindow GLFWwindow;

class Texture;

class GL3Renderer : public Renderer {
   public:
      virtual const char* get_APIName() override { return "OpenGL 3.3"; };

      void Initialize() override;
      
      void BeginRender(bool toGBuffer = true) override;
      Status EndRender() override;
      void PresentRender() override;

      void CreateVertexBuffer(Model* model) override;
      void CreateShaderProgram(Shader* shader) override;
      void CreateTextureBuffer(Texture* texture) override;

      virtual void CreateConObjects(Console* console) override;

      void InitImGui() override;
      void BeginImGui() override;
      void EndImGui() override;

      void DrawImGuiWindow() override;

      bool gbufferInited = false;
      uint gbufferFBO, gbufferDepthRBO;
      Texture* gbufferPositionTex, *gbufferNormalTex, *gbufferAlbedoTex, *gbufferEmissionTex;
      uint gbufferPositionID, gbufferNormalID, gbufferAlbedoID, gbufferEmissionID;

      void CreateGBuffers();

      void DrawLightingQuad() override; 
};

extern std::string gl3ErrorShaderCode; // Fallback shader that is used in place of a nullptr

#endif
