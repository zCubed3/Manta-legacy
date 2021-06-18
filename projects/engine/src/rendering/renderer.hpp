#ifndef MANTA_RENDERER_H
#define MANTA_RENDERER_H

#include <manta_macros.hpp>
#include <console/console.hpp>

#include <assets/model.hpp>
#include <assets/shader.hpp>

#include <entities/camera.hpp>

class Renderer;
class World;
class GLFWwindow;
class Resources;

// Technically a Color4 but it works
// Default color is #0d2342
typedef struct ClearColor_T {
   float r = RGB_TO_LINEAR(13);
   float g = RGB_TO_LINEAR(35);
   float b = RGB_TO_LINEAR(66);
   float a = 1.0f;
} ClearColor;

class Renderer {
   public:
      virtual const char* get_APIName() { return "Unknown API"; };


   // Renderer state
   public:
      enum class Status {
	 Running 	= 0,

	 Failure	= 1,
	 ShuttingDown 	= 2,
      };

   // Actual renderer functionality
   public:
      virtual void Initialize() = 0;

      virtual void BeginRenderShadow() = 0;
      virtual void EndRenderShadow() = 0;

      virtual void BeginRender() = 0;
      virtual Status EndRender() = 0;

      virtual void CreateConObjects(Console* console) {
	 if (console != nullptr) {
	    console->CreateCVar("r_window_resizable", "false");

	    console->CreateCVar("r_shadowmap_width", "512");
	    console->CreateCVar("r_shadowmap_height", "512");

	    console->CreateCVar("r_msaa_samples", "1");

	    console->CreateCVar("width", "1024");
	    console->CreateCVar("height", "768");
	    console->CreateCVar("fullscreen", "false");

	    this->console = console;
	 }
      };

      Console* console = nullptr; // Allows the renderer to read from a console
      ClearColor clearColor;

      // Initializes GPU assets
      virtual void CreateVertexBuffer(Model* model) = 0;
      virtual void CreateShaderProgram(Shader* shader) = 0;

      // ImGui
      virtual void InitImGui() = 0;
      virtual void BeginImGui() = 0;
      virtual void EndImGui() = 0;

      World* world;
      Resources* resources;

      float windowWidth = 100;
      float windowHeight = 100;
      bool vsync = true;

      Camera* camera;

      std::vector<Model*> modelQueue;

      GLFWwindow* window;

      bool showImGuiWindow = false;
      virtual void DrawImGuiWindow() = 0;
};

#endif
