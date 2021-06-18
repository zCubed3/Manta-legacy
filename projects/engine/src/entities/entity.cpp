#include <entities/entity.hpp>

#include <stdio.h>

#include <assets/model.hpp>
#include <assets/resources.hpp>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <entities/world.hpp>

#include <glm/glm/gtc/type_ptr.hpp>

void Entity::Update(World* world) {
   if (!isEnabled)
      return;

   rotation = glm::quat(glm::radians(euler));

   mModel = glm::translate(glm::mat4(1.0f), position);
   mModel *= glm::toMat4(rotation);
   mModel = glm::scale(mModel, scale);
}

void Entity::Draw(Renderer* renderer, Resources* resources) {
   if (!isVisible)
      return;

   for (int m = 0; m < models.size(); m++) {
      if (!models[m])
	 continue;

      if (!*models[m]) {
	 printf("Model #%i in entity is a nullptr!\n", m);
	 continue;
      }

      (*models[m])->Draw(renderer, resources, this);
   }
}

void Entity::DrawImGui(World* world, int index) {
   ImGui::PushID("entity_editor_"); ImGui::PushID(index);
   if (ImGui::TreeNode(name.c_str())) {
      ImGui::TreePop();
   }
   ImGui::PopID(); ImGui::PopID();

}
