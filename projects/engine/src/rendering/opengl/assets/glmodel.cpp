#include "glmodel.hpp"
#include "glshader.hpp"

#include <GL/glew.h>

#include <assets/shader.hpp>
#include <rendering/renderer.hpp>

#include <entities/world.hpp>

#include <glm/glm/gtc/type_ptr.hpp>

#include <rendering/opengl/gl3_renderer.hpp>
#include <string>

GL3VertexBuffer::~GL3VertexBuffer() {
   if (vao != GL_INVALID_INDEX)
      glDeleteVertexArrays(1, &vao);

   if (vbo != GL_INVALID_INDEX)
      glDeleteBuffers(1, &vbo);

   if (ibo != GL_INVALID_INDEX)
      glDeleteBuffers(1, &ibo);
}

void GL3VertexBuffer::Populate(Model* model) {
   printf("Creating OpenGL3 VBO for %s\n", model->name.c_str());

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex) * model->vertices.size(), model->vertices.data(), GL_STATIC_DRAW); 

   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * model->triangles.size(), model->triangles.data(), GL_STATIC_DRAW);

   indices = model->triangles.size();

   glBindVertexArray(0);
}

void GL3VertexBuffer::Draw(Renderer* renderer, Entity* entity, Shader* shader) {
   if (shader)
      shader->Bind();
   else
      return;

   GL3ShaderProgram* shaderProgram = dynamic_cast<GL3ShaderProgram*>(shader->program);
   GL3Renderer* glRenderer = dynamic_cast<GL3Renderer*>(renderer);

   if (shaderProgram) {
      uint program = shaderProgram->program;
      int location;

      location = shaderProgram->getUniform("MANTA_tex0");
      glUniform1i(location, 0);

      location = shaderProgram->getUniform("MANTA_tex1");
      glUniform1i(location, 1);

      location = shaderProgram->getUniform("MANTA_tex2");
      glUniform1i(location, 2);

      location = shaderProgram->getUniform("MANTA_tex3");
      glUniform1i(location, 3);

      if (entity) {
	 location = shaderProgram->getUniform("MANTA_mM");
	 glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(entity->mModel));

	 location = shaderProgram->getUniform("MANTA_mV");
	 glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(renderer->camera->mView));

	 location = shaderProgram->getUniform("MANTA_mP");
	 glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(renderer->camera->mPerspective));
      }

      if (renderer->camera) {
	 location = shaderProgram->getUniform("MANTA_pCamera");
	 glUniform3fv(location, 1, glm::value_ptr(renderer->camera->position));
      }

      if (renderer->world) {
	 char locationString[64];
	 sprintf(locationString, "MANTA_lightCount");
	    
	 int lightCtLocation = glGetUniformLocation(program, locationString);
	 glUniform1i(lightCtLocation, renderer->world->data.lightCount);

	 sprintf(locationString, "MANTA_ambientColor");

	 int ambientColLocation = glGetUniformLocation(program, locationString);
	 glUniform3fv(ambientColLocation, 1, glm::value_ptr(renderer->world->data.ambientColor));

	 for (int l = 0; l < renderer->world->data.lightCount; l++) {
	    sprintf(locationString, "MANTA_lightPositions[%i]", l);
	    
	    int lightPosLocation = glGetUniformLocation(program, locationString);
	    glUniform3fv(lightPosLocation, 1, glm::value_ptr(renderer->world->data.lightPositions[l]));

	    sprintf(locationString, "MANTA_lightDirections[%i]", l);
	    
	    int lightDirLocation = glGetUniformLocation(program, locationString);
	    glUniform3fv(lightDirLocation, 1, glm::value_ptr(renderer->world->data.lightDirections[l]));

	    sprintf(locationString, "MANTA_lightColors[%i]", l);
	    
	    int lightColLocation = glGetUniformLocation(program, locationString);
	    glUniform3fv(lightColLocation, 1, glm::value_ptr(renderer->world->data.lightColors[l]));

	    sprintf(locationString, "MANTA_lightRanges[%i]", l);
	    
	    int lightRangeLocation = glGetUniformLocation(program, locationString);
	    glUniform1f(lightRangeLocation, renderer->world->data.lightRanges[l]);

	    sprintf(locationString, "MANTA_lightIntensities[%i]", l);
	    
	    int lightIntensitiesLocation = glGetUniformLocation(program, locationString);
	    glUniform1f(lightIntensitiesLocation, renderer->world->data.lightIntensities[l]);

	    sprintf(locationString, "MANTA_lightParams1[%i]", l);
	    
	    int lightP1Location = glGetUniformLocation(program, locationString);
	    glUniform1f(lightP1Location, renderer->world->data.lightParams1[l]);

	    sprintf(locationString, "MANTA_lightParams2[%i]", l);
	    
	    int lightP2Location = glGetUniformLocation(program, locationString);
	    glUniform1f(lightP2Location, renderer->world->data.lightParams2[l]);

	    sprintf(locationString, "MANTA_lightTypes[%i]", l);
	    
	    int lightTypeLocation = glGetUniformLocation(program, locationString);
	    glUniform1i(lightTypeLocation, renderer->world->data.lightTypes[l]);
	 }

	 glUniform1f(glGetUniformLocation(program, "MANTA_fTime"), renderer->world->timeTotal);
      }
   }

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);

   int vertSize = sizeof(Model::Vertex);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertSize, nullptr);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertSize, (void*)(sizeof(float) * 3));
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertSize, (void*)(sizeof(float) * 6));

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);

   glBindVertexArray(0);
}
