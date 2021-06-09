#ifndef MANTA_RENDERER_H
#define MANTA_RENDERER_H

#include "manta_macros.hpp"

class Renderer;

MANTA_DECLARE_FPTR(Renderer*, FuncGetRenderer, );

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
      enum class StatusFlag {
	 Success     = MANTA_FLAG(0),

	 Failure     = MANTA_FLAG(1),
      };

      struct RendererState {
	 Renderer::StatusFlag status;
	 const char* failReason;
      };

   // Actual renderer functionality
   public:
      virtual void Initialize() = 0;
      virtual RendererState Render() = 0;
};

#endif
