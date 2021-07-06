#include "entities/entity.hpp"
#include <rendering/renderer.hpp>

#include <manta_macros.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <deque>

#include <assets/manta_packer.hpp>
#include <assets/model.hpp>
#include <assets/shader.hpp>
#include <assets/resources.hpp>

#include <console/console.hpp>
#include <console/common_console.hpp>

#include <entities/world.hpp>
#include <entities/light.hpp>

#include <common/spinner.hpp>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <glm/glm/gtc/quaternion.hpp>

#include <rendering/opengl/gl3_renderer.hpp>

int main(int argc, char** argv) {
   Console console;
   Resources resources;
   World world;

   world.resources = &resources;
   resources.world = &world; // Circular, I know

   CreateCommonConObjects(&console);
   world.CreateConObjects(&console);
   //PackerCreateConObjects(&console);
 
   Spinner spinner;

   Renderer* renderer = new GL3Renderer();

   if (renderer == nullptr) {
      printf("Fatal: Failed to create renderer\n");
      exit(0);
   }

   renderer->resources = &resources;
   renderer->CreateConObjects(&console);

   resources.renderer = renderer;

   console.ParseExecFile("./data/autoexec"); // Autoexec is the default autoexec path
   console.ParseCommandLine(argc, argv);

   console.CVarGetData("gamename", "Test");

   renderer->world = &world;
   renderer->Initialize();

   Camera camera;
   camera.position = glm::vec3(0, 0, 3);
   camera.euler = glm::vec3(0, 180, 0);
   camera.ignoreConFov = false;
   camera.renderer = renderer;
   camera.isProtected = true;
   camera.name = "Scene Camera";

   renderer->camera = &camera;

   world.entities.emplace_back(&camera);

   Light testLight1, testLight2, testLight3;
   std::vector<Light*> testLights = { &testLight1, &testLight2, &testLight3 };

   for (int l = 0; l < testLights.size(); l++) {
      testLights[l]->type = Light::LightType::Point;
      world.entities.emplace_back(testLights[l]);

      testLights[l]->scale = glm::vec3(1, 1, 1) * 0.1f;
      testLights[l]->name = "Orbit Light";
   }

   testLight1.color = glm::vec3(1, 0, 0);
   testLight2.color = glm::vec3(0, 1, 0);
   testLight3.color = glm::vec3(0, 0, 1);

   Light cameraSpot;
   cameraSpot.color = glm::vec3(1, 1, 1);
   cameraSpot.type = Light::LightType::Spot;
   cameraSpot.range = 5.0f;

   cameraSpot.param1 = 10.0f;
   cameraSpot.param2 = 50.0f;

   cameraSpot.position = glm::vec3(0, 3, 0);
   cameraSpot.euler = glm::vec3(90, 0, 0);

   //world.entities.emplace_back(&cameraSpot);

   Renderer::Status state;
   
   float pi = 3.1415926535f;
   float piThird = 2.094400006763f;

   double mouseX, mouseY;
   glfwGetCursorPos(renderer->window, &mouseX, &mouseY);
      
   bool lockCursor = false, hasLocked = false;
   bool forward = false, backward = false;

   ImGui::CreateContext();
   renderer->InitImGui();
   ImGuiStyle& style = ImGui::GetStyle();

   ImGui::StyleColorsDark(&style);

   style.WindowRounding = 1.0f;
   
   ImGuiIO& imguiIO = ImGui::GetIO();
   imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

   while (true) {
      glfwPollEvents();

      world.Update();
	 
      for (int l = 0; l < testLights.size(); l++) {
	 int o = l + 1;
	 testLights[l]->position = glm::vec3(sinf(world.timeTotal + piThird * o) * 3, cosf(world.timeTotal + piThird * o) * 3, sinf(world.timeTotal));
	 testLights[l]->euler += glm::vec3(0, 1, 0);
      }

      //RGB light
      //testLight1.color = glm::vec3(abs(sinf(renderer->timeTotal)), abs(sinf(renderer->timeTotal + 1)), abs(sinf(renderer->timeTotal + 2)));

      //
      // Very shitty free camera code :tm:
      //
      double nowMouseX, nowMouseY;
      glfwGetCursorPos(renderer->window, &nowMouseX, &nowMouseY);
	 
      if (lockCursor) {
	 camera.euler += glm::vec3(nowMouseY - mouseY, mouseX - nowMouseX, 0) * 0.1f;
      
	 if (glfwGetKey(renderer->window, GLFW_KEY_W))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(0, 0, 1)) * world.timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_S))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(0, 0, -1)) * world.timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_A))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(1, 0, 0)) * world.timeDelta;

	 if (glfwGetKey(renderer->window, GLFW_KEY_D))
	    camera.position += glm::rotate(camera.rotation, glm::vec3(-1, 0, 0)) * world.timeDelta;
      }

      if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !hasLocked) {
	 lockCursor = !lockCursor;
	 glfwSetInputMode(renderer->window, GLFW_CURSOR, lockCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	 hasLocked = true;
      }
	 
      if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	 hasLocked = false;

      mouseX = nowMouseX;
      mouseY = nowMouseY;

      cameraSpot.position = camera.position;
      cameraSpot.euler = camera.euler;

      //testEnt.euler += glm::vec3(1, 0, 0);

      //printf("\rRunning %c", spinner.character);
      //spinner.Spin();

      // Render GBuffers
      renderer->BeginRender(Renderer::RenderType::GBuffer);

      world.Draw(renderer);

      if (renderer->EndRender() != Renderer::Status::Running) {
	 break;
      }
      
      // Then render the default scene
      renderer->BeginRender(Renderer::RenderType::Default);
      renderer->DrawLightingQuad();

      if (renderer->EndRender() != Renderer::Status::Running) {
	 break;
      }

      if (!lockCursor) {
	 renderer->BeginImGui();
      
	 ImGui::Begin("Windows");
	 
	 ImGui::Checkbox("Renderer", &renderer->showImGuiWindow);
	 ImGui::Checkbox("World", &world.showWindow);
	 ImGui::Checkbox("Resources", &resources.showWindow);

	 ImGui::End();

	 world.DrawImGuiWindow();
	 renderer->DrawImGuiWindow();
	 resources.DrawImGuiWindow();

	 renderer->EndImGui();
      }
      
      renderer->PresentRender();
   }
}

