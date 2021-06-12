#ifndef MANTA_RENDERER_H
#define MANTA_RENDERER_H

#include "manta_macros.hpp"
#include "console.hpp"

class Renderer;

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
      // For determining which API we're using, for diagnotistic and stat reasons
      enum class RenderAPI {
	 Unknown = 0,

	 OpenGL	 = 1,
	 Vulkan  = 2,

#ifdef _WIN32
	 // Futureproofing
	 DX11	 = 3,
	 DX12    = 4,
#endif
      };

      virtual RenderAPI get_RenderAPI() { return RenderAPI::Unknown; };
      virtual const char* get_RenderAPIName(RenderAPI api) { 
	 switch (api) {
	    default:
	       return "Unknown API";

	    case RenderAPI::OpenGL:
	       return "OpenGL";

	    case RenderAPI::Vulkan:
	       return "Vulkan";

#ifdef _WIN32
	    case RenderAPI::DX11
	       return "DirectX 11";

	    case RenderAPI::DX12
	       return "DirectX 12";
#endif
	 }
      };


   // Renderer state
   public:
      enum class Status {
	 Failure     = 0,

	 Success     = 1,

	 ShutDown    = 2,
      };

      struct RendererState {
	 Renderer::Status status;
	 const char* failReason;
      };

   // Actual renderer functionality
   public:
      virtual void Initialize() = 0;
      virtual RendererState Render() = 0;

      virtual void RegisterConObjects() {
	 if (console != nullptr) {
	    console->CreateCVar("r_window_resizable", "false");
	    
	    console->CreateCVar("r_window_width", "1024");
	    console->CreateCVar("r_window_height", "768");
	 }
      };

      ConsoleInstance* console = nullptr; // Allows the renderer to read from a console
      ClearColor clearColor;
};

#endif
