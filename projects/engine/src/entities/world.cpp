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

   if (ImGui::Button("Add Light"))
      entities.emplace_back(new Light());

   if (ImGui::Button("Add Entity"))
      entities.emplace_back(new Entity());

   if (ImGui::TreeNode("World Settings")) {
      ImGui::DragFloat3("Ambient Color", glm::value_ptr(data.ambientColor), timeDelta, 0, 1);
      ImGui::TreePop();
   }

   if (ImGui::TreeNode("World Info")) {
      ImGui::Text("Lights: %i", data.lightCount);
      ImGui::TreePop();
   }

   int deletion = -1;
   if (ImGui::TreeNode("Entities")) {
      int e = 0;
      for (const auto& entity : entities) {
	 if (entity == nullptr) {
	    continue;
	 }
   
	 char entityName[256];
	 sprintf(entityName, "%s##%i", entity->name.c_str(), e);

	 if (ImGui::TreeNode(entityName)) {
	    if (!entity->isProtected)
	       if (ImGui::Button("Delete"))
		  deletion = e;

	    ImGui::DragFloat3("Position", glm::value_ptr(entity->position), timeDelta);
	    ImGui::DragFloat3("Euler", glm::value_ptr(entity->euler), timeDelta);
	    ImGui::DragFloat3("Scale", glm::value_ptr(entity->scale), timeDelta);

	    if (ImGui::InputText("Name", &entity->nameBuffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
	       entity->name = entity->nameBuffer;
	       entity->nameBuffer.clear();
	    }	

	    if (ImGui::Button("Add Model"))
	       entity->models.push_back(nullptr);

	    char modelsId[56];
	    sprintf(modelsId, "Models##%s_%i", entity->name.c_str(), e);

	    if (ImGui::TreeNode(modelsId)) {
	       int i = 0;
	       for (auto& model : entity->models) {
		  const char* modelName = "None";

		  if (model)
		     if (*model)
			modelName = (*model)->name.c_str();

		  char modelText[56];
		  sprintf(modelText, "%s##%s_%i_%i", modelName, entity->name.c_str(), e, i);

		  if (ImGui::Button("X")) {
		     entity->models.erase(entity->models.begin() + i);
		     break;
		  }

		  ImGui::SameLine();
		  if (ImGui::BeginCombo(modelText, modelName)) {
		     for (auto& loadedModel : resources->modelLoader.loadedModels) {
			if (!loadedModel.second)
			   continue;

			bool isSelected = modelName == loadedModel.second->name;

			if (ImGui::Selectable(loadedModel.second->name.c_str(), isSelected))
			   model = &loadedModel.second;

			if (isSelected)
			   ImGui::SetItemDefaultFocus();
		     }
		     ImGui::EndCombo();
		  }

		  i++;
	       }
	       ImGui::TreePop();
	    }

	    Light* light = dynamic_cast<Light*>(entity);
	    if (light) {
	       ImGui::InputInt("Type", reinterpret_cast<int*>(&light->type), 1);
	       ImGui::ColorEdit3("Color", glm::value_ptr(light->color));
	       ImGui::DragFloat("Range", &light->range, timeDelta);
	       ImGui::DragFloat("Intensity", &light->intensity, timeDelta);
	       ImGui::DragFloat("Param 1", &light->param1, timeDelta);
	       ImGui::DragFloat("Param 2", &light->param2, timeDelta);
	    }

	    ImGui::TreePop();
	 }

	 e++;
      }
      ImGui::TreePop();
   }

   if (deletion >= 0)
      entities.erase(entities.begin() + deletion);

   ImGui::End();
}
