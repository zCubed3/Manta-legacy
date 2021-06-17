#include "resources.hpp"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <rendering/renderer.hpp>

void Resources::DrawImGuiWindow() {
   if (!showWindow)
      return;

   ImGui::Begin("Resources");

   ImGui::InputText("Path", &inputBuffer);

   if (ImGui::Button("Load Model")) {
      Model* model = modelLoader.LoadModel(inputBuffer);
   
      if (model) {
	 model->shader = shaderLoader.LoadShader("engine#error");
	 renderer->CreateVertexBuffer(model);
      }
   }

   if (ImGui::Button("Load Shader")) {
      Shader* shader = shaderLoader.LoadShader(inputBuffer);
   
      if (shader)
	 renderer->CreateShaderProgram(shader);
   }

   if (ImGui::TreeNode("Loaded Models")) {
      for (auto& model : modelLoader.loadedModels) {
	 if (!model.second)
	    continue;

	 if (ImGui::TreeNode(model.second->name.c_str())) {
	    if (ImGui::BeginCombo("Shader", model.second->shader->name.c_str())) {
	       for (auto& shader : shaderLoader.shaders) {
		  if (!shader.second)
		     continue;

		  bool isSelected = model.second->shader == shader.second;
		  if (ImGui::Selectable(shader.first.c_str(), isSelected))
		     model.second->shader = shader.second;

		  if (isSelected)
		     ImGui::SetItemDefaultFocus();
	       }

	       ImGui::EndCombo();
	    }

	    ImGui::TreePop();
	 }
      }
      
      ImGui::TreePop();
   }

   ImGui::End();
}
