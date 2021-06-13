#include <dynamiclib.hpp>
#include <renderer.hpp>

#include <manta_macros.hpp>
#include <manta_errcodes.hpp>
#include <console.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <assets/model.hpp> // REMOVE ME!

#include <common/common_console.hpp>

int main(int argc, char** argv) {
   ConsoleInstance console;

   CreateCommonConObjects(&console);

   DynamicLib* rendererLib = LoadDynamicLib("./lib/OpenGL3_api");
      
   if (rendererLib) {
      FuncGetRenderer funcGetRenderer = (FuncGetRenderer)rendererLib->GetFunction("get_Renderer");
      Renderer* renderer = nullptr;

      if (funcGetRenderer != nullptr)
	 renderer = funcGetRenderer();

      if (renderer == nullptr) {
	 printf("Failed to create renderer\n");
	 return ERR_FAILED_TO_CREATE_RENDERER;
      }

      renderer->console = &console;
      renderer->RegisterConObjects();
   
      console.ParseAutoExec();
      console.ParseCommandLine(argc, argv);

      console.CVarGetData("gamename", "Test");

      renderer->Initialize();

      Model* testModel = renderer->modelLoader.LoadModel("./data/models/Sphere.obj"); // Don't make me hardcoded!
      renderer->CreateBuffer(testModel);
      renderer->modelQueue.emplace_back(testModel);

      Shader* testShader = renderer->shaderLoader.LoadShader("./data/shaders/Test.glsl");
      renderer->CreateShaderProgram(testShader);

      Camera camera;
      renderer->camera = &camera;

      Renderer::RendererState state;
      while (true) {
	 testShader->Bind(renderer);
	 camera.Update(renderer);
	 state = renderer->Render();

	 // TODO: Make this be more verbose
	 if (state.status != Renderer::Status::Success)
	    break;

      }
   }
}

