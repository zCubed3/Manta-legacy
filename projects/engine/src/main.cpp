#include <dynamiclib.hpp>
#include <renderer.hpp>

#include <manta_macros.hpp>
#include <console.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <assets/manta_packer.hpp>
#include <assets/model.hpp> // REMOVE ME!

#include <common/common_console.hpp>

#include <entities/world.hpp>
#include <entities/light.hpp>

#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
   Console console;
   World world;

   CreateCommonConObjects(&console);
   world.CreateConObjects(&console);
   //PackerCreateConObjects(&console);

   console.CreateCVar("testmodel", "./data/models/Sphere.obj");
   console.CreateCVar("testshader", "./data/shaders/Standard.glsl");

   DynamicLib* rendererLib = LoadDynamicLib("./lib/OpenGL3_api");
      
   if (rendererLib) {
      FuncGetRenderer funcGetRenderer = (FuncGetRenderer)rendererLib->GetFunction("get_Renderer");
      Renderer* renderer = nullptr;

      if (funcGetRenderer != nullptr)
	 renderer = funcGetRenderer();

      if (renderer == nullptr) {
	 printf("Fatal: Failed to create renderer\n");
	 exit(0);
      }

      renderer->CreateConObjects(&console);

      console.ParseExecFile("./data/autoexec"); // Autoexec is the default autoexec path
      console.ParseCommandLine(argc, argv);

      console.CVarGetData("gamename", "Test");

      renderer->world = &world;

      renderer->Initialize();

      Model* testModel = renderer->modelLoader.LoadModel(console.CVarGetData("testmodel", ""));
      if (!testModel) {
	 printf("Fatal: TestModel wasn't loaded, aborting!\n");
	 exit(0);
      }

      //MantaPackModel(testModel, "test.mmdl");

      renderer->CreateBuffer(testModel);
      renderer->modelQueue.emplace_back(testModel);

      Shader* testShader = renderer->shaderLoader.LoadShader(console.CVarGetData("testshader", ""));
      //Shader* testShader = renderer->shaderLoader.LoadCode("engine#error");

      if (!testShader) {
	 printf("Fatal: TestShader wasn't loaded, aborting!\n");
	 exit(0);
      }

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

      Renderer::Status state;
   
      float pi = 3.1415926535f;
      float piThird = 2.094400006763f;

      double mouseX, mouseY;
      glfwGetCursorPos(renderer->window, &mouseX, &mouseY);
      
      bool lockCursor = false, hasLocked = false;
      bool forward = false, backward = false;
      while (true) {
	 world.Update();
	 state = renderer->Render();

	 testLight1.position = glm::vec3(sinf(renderer->timeTotal + piThird) * 3, cosf(renderer->timeTotal + piThird) * 3, sinf(renderer->timeTotal));
	 testLight2.position = glm::vec3(sinf(renderer->timeTotal + piThird * 2) * 3, cosf(renderer->timeTotal + piThird * 2) * 3, sinf(renderer->timeTotal));
	 testLight3.position = glm::vec3(sinf(renderer->timeTotal + piThird * 3) * 3, cosf(renderer->timeTotal + piThird * 3) * 3, sinf(renderer->timeTotal));

	 // RGB light
	 //testLight1.color = glm::vec3(abs(sinf(renderer->timeTotal)), abs(sinf(renderer->timeTotal + 1)), abs(sinf(renderer->timeTotal + 2)));

	 //
	 // Very shitty free camera code :tm:
	 //
	 double nowMouseX, nowMouseY;
	 glfwGetCursorPos(renderer->window, &nowMouseX, &nowMouseY);
	 
	 if (lockCursor)
	    camera.euler += glm::vec3(nowMouseY - mouseY, mouseX - nowMouseX, 0) * 0.1f;

	 if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !hasLocked) {
	    lockCursor = !lockCursor;
	    glfwSetInputMode(renderer->window, GLFW_CURSOR, lockCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	    hasLocked = true;
	 }
	 
	 if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	    hasLocked = false;

	 if (glfwGetKey(renderer->window, GLFW_KEY_W))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(0, 0, 1)) * renderer->timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_S))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(0, 0, -1)) * renderer->timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_A))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(1, 0, 0)) * renderer->timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_D))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(-1, 0, 0)) * renderer->timeDelta;

	 mouseX = nowMouseX;
	 mouseY = nowMouseY;

	 if (state != Renderer::Status::Running) {
	    break;
	 }
      }
   }
}

