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

#include <entities/world.hpp>
#include <entities/light.hpp>

int main(int argc, char** argv) {
   ConsoleInstance console;
   World world;

   CreateCommonConObjects(&console);

   console.CreateCVar("testmodel", "./data/models/Sphere.obj");
   console.CreateCVar("testshader", "./data/shaders/Test.glsl");

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

      renderer->world = &world;

      renderer->Initialize();

      Model* testModel = renderer->modelLoader.LoadModel(console.CVarGetData("testmodel", ""));
      renderer->CreateBuffer(testModel);
      renderer->modelQueue.emplace_back(testModel);

      Shader* testShader = renderer->shaderLoader.LoadShader(console.CVarGetData("testshader", ""));
      renderer->CreateShaderProgram(testShader);

      testModel->shader = testShader;

      Camera camera;
      camera.renderer = renderer;

      renderer->camera = &camera;

      world.entities.emplace_back(&camera);

      Entity testEnt;

      testEnt.models.emplace_back(testModel);
      world.entities.emplace_back(&testEnt);

      Light testLight;
      testLight.position = glm::vec3(0, 1, 0);

      world.entities.emplace_back(&testLight);

      Renderer::RendererState state;
      while (true) {
	 world.Update();
	 state = renderer->Render();

	 // TODO: Make this be more verbose
	 if (state.status != Renderer::Status::Success)
	    break;

      }
   }
}

