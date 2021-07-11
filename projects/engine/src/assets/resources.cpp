#include "resources.hpp"

#include <filesystem>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <rendering/renderer.hpp>

#include <console/console.hpp>

#include <entities/entity.hpp>
#include <entities/world.hpp>

using namespace std::filesystem;

Resources::Resources() {
   FindDataPaths();   
}

void IterateThroughPath(ResourcesPath& path) {
   for (auto& file : directory_iterator(path.path)) {
      if (file.is_directory()) {
	 ResourcesPath subPath;
	 subPath.path = file.path();
	 
	 IterateThroughPath(subPath);
	 path.childPaths.emplace_back(subPath);
      }
      else {
	 path.files.emplace_back(file);
	 //printf("File: %s\n", file.path().c_str());
      }
   }
}

void Resources::FindDataPaths() {
   // Get surface level directories
   ResourcesPath surfacePath;
   surfacePath.path = "./data";
   for (auto& file : directory_iterator(surfacePath.path)) {
      if (file.is_directory()) {
	 ResourcesPath resPath;
	 resPath.path = file.path();
	 IterateThroughPath(resPath);
	 surfacePath.childPaths.emplace_back(resPath);
      }
      else
	 surfacePath.files.emplace_back(file); 
   }

   dataPaths.emplace_back(surfacePath);
}

Model* Resources::LoadModel(std::string path) {
   Model* model = modelLoader.LoadModel(path);
   
   if (model) {
      if (model->shader == nullptr)
	 model->shader = shaderLoader.GetShader("engine#error");
      
      renderer->CreateVertexBuffer(model);
   }

   return model;
}

Shader* Resources::LoadShader(std::string path) {
   Shader* shader = shaderLoader.LoadShader(path);
   
   if (shader)
      renderer->CreateShaderProgram(shader);
      
   return shader;
}

Texture* Resources::LoadTexture(std::string path) {
   // If path contains _3D, it's a cubemap
   
   Texture::TextureType type = Texture::TextureType::Texture2D;
   if (strstr(path.c_str(), "_3D"))
      type = Texture::TextureType::Texture3D;

   Texture* texture = textureLoader.LoadFromFile(path, type);

   if (texture)
      renderer->CreateTextureBuffer(texture);

   return texture;
}

void Resources::TryLoadFromExtension(std::string path, std::string extension) {
   if (extension == ".obj" || extension == ".mmdl")
      LoadModel(path);

   if (extension == ".glsl")
      LoadShader(path);

   if (extension == ".jpg" || extension == ".png" || extension == ".bmp")
      LoadTexture(path);
}

void DrawImGuiPaths(Resources* resources, ResourcesPath path) {
   std::string name = "UNKNOWN_FILE";

   if (path.path.string() == "./")
      name = "MANTA_ROOT";
   else
      name = path.path.filename().c_str();

   ImGui::PushID(path.path.c_str());
   if (ImGui::TreeNode(name.c_str())) {
      for (auto& subPath : path.childPaths) {
	 DrawImGuiPaths(resources, subPath);
      }

      for (auto& file : path.files) {
	 auto extension = file.extension();

	 ImGui::PushID("load_"); ImGui::PushID(path.path.c_str());
	 if (ImGui::Button(file.filename().c_str())) {
	    //printf("Path: %s\n", file.c_str());
	    resources->TryLoadFromExtension(file.c_str(), extension.c_str());
	 }
	 ImGui::PopID(); ImGui::PopID();
      }

      ImGui::TreePop();
   }
   ImGui::PopID();
}

void Resources::DrawImGuiWindow() {
   if (!showWindow)
      return;

   ImGui::Begin("Resources");

   ImGui::InputText("Path", &inputBuffer);

   if (ImGui::Button("Load Model##resources_window_load_model")) {
      LoadModel(inputBuffer);
   }

   if (ImGui::Button("Load Shader##resources_window_load_shader")) {
      LoadShader(inputBuffer);
   }

   if (ImGui::TreeNode("Explorer##resources_window_explorer")) {
      for (auto& path : dataPaths)
	 DrawImGuiPaths(this, path);

      ImGui::TreePop();
   }

   if (ImGui::TreeNode("Loaded Models##resources_window_loaded_models")) {
      for (auto& model : modelLoader.loadedModels) {
	 if (!model.second)
	    continue;

	 const char* shaderName = "None";
	 if (model.second->shader)
	    shaderName = model.second->shader->name.c_str();

	 if (ImGui::TreeNode(model.second->name.c_str())) {
	    ImGui::PushID(model.first.c_str());
	    if (ImGui::Button("Create Entity##resources_window_loaded_models_create_ent_")) { // Creates an empty entity with this model inside it
	       Entity* modelEnt = new Entity();
	       modelEnt->models.emplace_back(&model.second);
	       modelEnt->name = model.second->name;
	       world->entities.emplace_back(modelEnt);
	    }
	    ImGui::PopID();

	    if (ImGui::BeginCombo("Shader", shaderName)) {
	       for (auto& shader : shaderLoader.shaders) {
		  if (!shader.second)
		     continue;

		  bool isSelected = false;

		  if (model.second->shader)
		     isSelected = model.second->shader == shader.second;
		  
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

   if (ImGui::TreeNode("Loaded Textures##resources_window_loaded_textures")) {
      for (auto& texture : textureLoader.loadedTextures) {
	 ImGui::Image((void*)(intptr_t)texture.second->texBuffer->handle, ImVec2(100, 100));
      }
      ImGui::TreePop();
   }

   ImGui::End();
}
