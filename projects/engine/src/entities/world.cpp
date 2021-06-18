#include <entities/world.hpp>

#include <stdio.h>

#include <assets/resources.hpp>
#include <assets/model.hpp>

#include <entities/light.hpp>

#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <imgui/misc/cpp/imgui_stdlib.h>

void World::CreateConObjects(Console* console) {
   if (console) {
      //
      // CFuncs
      //

      //console->Create
      //
      this->console = console;
   }
}

void World::Update() {   
   int l = 0;
   for (int e = 0; e < entities.size(); e++) {
      if (entities[e] == nullptr) {
	 printf("Entity #%i is a nullptr\n", e);
	 continue;
      }

      Light* light = dynamic_cast<Light*>(entities[e]);

      entities[e]->Update(this);

      if (light != nullptr) {
	 data.lightPositions[l] = light->position;
	 data.lightDirections[l] = glm::rotate(light->rotation, glm::vec3(0, 0, 1));
	 data.lightColors[l] = light->color;
	 data.lightRanges[l] = light->range;
	 data.lightIntensities[l] = light->intensity;
	 data.lightParams1[l] = light->param1;
	 data.lightParams2[l] = light->param2;
	 data.lightTypes[l] = (int)light->type;
	 l++;	 
      }	 
   }

   data.lightCount = l;
}

void World::Draw(Renderer* renderer) {
   timeTotal = (float)glfwGetTime();
   timeDelta = timeTotal - timeLast;
   timeLast = timeTotal;

   for (int e = 0; e < entities.size(); e++) {
      if (entities[e] == nullptr)
	 continue;

      entities[e]->Draw(renderer, resources);
   }
}

void World::DrawImGuiWindow() {
   if (!showWindow)
      return;

   ImGui::Begin("World");

   if (ImGui::TreeNode("Create Entity##create_ent_menu")) {
      if (ImGui::Button("+ Light"))
	 entities.emplace_back(new Light());

      if (ImGui::Button("+ Entity"))
	 entities.emplace_back(new Entity());
   
      ImGui::TreePop();
   }

   if (ImGui::TreeNode("World Settings")) {
      ImGui::DragFloat3("Ambient Color", glm::value_ptr(data.ambientColor), timeDelta, 0, 1);
      
      if (ImGui::TreeNode("World Info")) {
	 ImGui::Text("Lights: %i", data.lightCount);
	 ImGui::TreePop();
      }

      ImGui::TreePop();
   }


   int deletion = -1;
   if (ImGui::TreeNode("Entities")) {
      int e = 0;
      for (const auto& entity : entities) {
	 ImGui::PushID("delete_ent_"); ImGui::PushID(e);
	 if (ImGui::Button("X")) {
	    ImGui::PopID(); ImGui::PopID();
	    deletion = e;
	    break;
	 }

	 if (entity == nullptr)
	    continue;
	
	 ImGui::SameLine();
	 entity->DrawImGui(this, e);
	 ImGui::PopID(); ImGui::PopID();

	 e++;
      }
      ImGui::TreePop();
   }

   if (deletion >= 0)
      entities.erase(entities.begin() + deletion);

   ImGui::End();
}
