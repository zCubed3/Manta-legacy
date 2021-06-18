#ifndef MANTAGL3_GL3RENDERER_H
#define MANTAGL3_GL3RENDERER_H

#include <rendering/renderer.hpp>

typedef unsigned int uint;

typedef struct GLFWwindow GLFWwindow;

class GL3Renderer : public Renderer {
   public:
      virtual const char* get_APIName() override { return "OpenGL 3.3"; };

      void Initialize() override;
      
      void BeginRenderShadow() override;
      void EndRenderShadow() override;

      void BeginRender() override;
      Status EndRender() override;

      void CreateVertexBuffer(Model* model) override;
      void CreateShaderProgram(Shader* shader) override;

      virtual void CreateConObjects(Console* console) override;

      void InitImGui() override;
      void BeginImGui() override;
      void EndImGui() override;

      void DrawImGuiWindow() override;

      uint shadowmapFBO, shadowmap;
      int shadowmapWidth, shadowmapHeight;
};

extern std::string gl3ErrorShaderCode; // Fallback shader that is used in place of a nullptr
extern std::string gl3GBufferShaderCode; // Writes data to the gbuffer which is used for lighting

#endif
