#include <assets/glvertexbuffer.hpp>

#include <GL/glew.h>

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

void GL3VertexBuffer::Draw() {
   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);

   int vec3Size = sizeof(glm::vec3);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vec3Size, nullptr);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vec3Size, (void*)vec3Size);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vec3Size, (void*)(vec3Size * 2));
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);

   glBindVertexArray(0);
}
