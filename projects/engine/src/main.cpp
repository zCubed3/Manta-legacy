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
   Console console;
   World world;

   world.console = &console;

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
      camera.position = glm::vec3(0, 0, 3);
      camera.euler = glm::vec3(0, 180, 0);
      camera.ignoreConFov = false;
      camera.renderer = renderer;

      renderer->camera = &camera;

      world.entities.emplace_back(&camera);

      Entity testEnt;

      testEnt.models.emplace_back(testModel);
      world.entities.emplace_back(&testEnt);

      Light testLight1, testLight2, testLight3;

      testLight1.color = glm::vec3(1, 0, 0);
      testLight2.color = glm::vec3(0, 1, 0);
      testLight3.color = glm::vec3(0, 0, 1);

      testLight1.type = testLight2.type = testLight3.type = Light::LightType::Point;

      world.entities.emplace_back(&testLight1);
      world.entities.emplace_back(&testLight2);
      world.entities.emplace_back(&testLight3);

      Renderer::RendererState state;
   
      float pi = 3.1415926535f;
      float piThird = 2.094400006763f;

      while (true) {
	 world.Update();
	 state = renderer->Render();

	 testLight1.position = glm::vec3(sinf(renderer->timeTotal + piThird) * 3, cosf(renderer->timeTotal + piThird) * 3, sinf(renderer->timeTotal));
	 testLight2.position = glm::vec3(sinf(renderer->timeTotal + piThird * 2) * 3, cosf(renderer->timeTotal + piThird * 2) * 3, sinf(renderer->timeTotal));
	 testLight3.position = glm::vec3(sinf(renderer->timeTotal + piThird * 3) * 3, cosf(renderer->timeTotal + piThird * 3) * 3, sinf(renderer->timeTotal));

	 // RGB light
	 //testLight1.color = glm::vec3(abs(sinf(renderer->timeTotal)), abs(sinf(renderer->timeTotal + 1)), abs(sinf(renderer->timeTotal + 2)));
	 
	 //camera.euler = glm::vec3(0, renderer->timeTotal, 0);

	 // TODO: Make this be more verbose
	 if (state.status != Renderer::Status::Success)
	    break;

      }
   }
}

