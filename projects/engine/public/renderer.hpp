#ifndef MANTA_RENDERER_H
#define MANTA_RENDERER_H

#include "manta_macros.hpp"
#include "console.hpp"

#include "assets/model.hpp"
#include "assets/shader.hpp"

#include "entities/camera.hpp"

class Renderer;
class World;
class GLFWwindow;

MANTA_DECLARE_FPTR(Renderer*, FuncGetRenderer, );

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
      virtual Status Render() = 0;

      virtual void CreateConObjects(Console* console) {
	 if (console != nullptr) {
	    console->CreateCVar("r_window_resizable", "false");
	    
	    console->CreateCVar("width", "1024");
	    console->CreateCVar("height", "768");
	    console->CreateCVar("fullscreen", "false");

	    this->console = console;
	 }
      };

      Console* console = nullptr; // Allows the renderer to read from a console
      ClearColor clearColor;

      // Rendering necessities
      virtual void CreateBuffer(Model* model) = 0;
      virtual void CreateShaderProgram(Shader* shader) = 0;

      World* world;

      ModelLoader modelLoader;
      ShaderLoader shaderLoader;

      float windowWidth = 1;
      float windowHeight = 1;

      float timeTotal = 0.0f;
      float timeLast = 0.0f;
      float timeDelta = 0.0f;

      Camera* camera;

      std::vector<Model*> modelQueue;

      GLFWwindow* window;
};

#endif
