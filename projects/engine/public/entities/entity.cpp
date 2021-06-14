#include <entities/entity.hpp>

#include <stdio.h>

#include <assets/model.hpp>

void Entity::Update() {
   if (!isEnabled)
      return;

   rotation = glm::quat(euler);

   mModel = glm::translate(glm::mat4(1.0f), position);
   mModel *= glm::toMat4(rotation);
   mModel = glm::scale(mModel, scale);
}

void Entity::Draw(Renderer* renderer) {
   if (!isVisible)
      return;

   for (int m = 0; m < models.size(); m++) {
      if (models[m] == nullptr) {
	 printf("Model #%i in entity is a nullptr!\n", m);
	 continue;
      }

      models[m]->Draw(renderer, this);
   }
}
