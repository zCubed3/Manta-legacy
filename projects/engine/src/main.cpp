#include <dynamiclib.hpp>
#include <renderer.hpp>

#include <manta_macros.hpp>
#include <manta_errcodes.hpp>
#include <console.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <assets/model.hpp> // REMOVE ME!

void ConTestFunction(std::vector<std::string> args) {
  printf("ConTestFunc()!\n");

  for (int a = 0; a < args.size(); a++)
     printf("%s\n", args[a].c_str());
}

int main(int argc, char** argv) {
   ConsoleInstance console;

   console.CreateCFunc("test_cmd", &ConTestFunction);
   console.CreateCVar("test_cvar", "test");   

   console.CreateCVar("gamename", "Manta");

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
   
      console.ParseCommandLine(argc, argv);

      renderer->Initialize();

      Model* test = renderer->modelLoader.LoadModel("./data/models/Cube.obj"); // Don't make me hardcoded!
      renderer->CreateBuffer(test);
      renderer->modelQueue.emplace_back(test);

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

