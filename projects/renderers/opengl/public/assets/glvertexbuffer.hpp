#ifndef MANTAGL3_GLVERTEXBUFFER_H
#define MANTAGL3_GLVERTEXBUFFER_H

#include <assets/model.hpp>

typedef unsigned int uint;

class GL3VertexBuffer : public VertexBuffer {
   public:
      void Populate(Model* model);
      void Draw(Renderer* renderer, Entity* entity, Shader* shader);
      ~GL3VertexBuffer();

   protected:
      uint vao, vbo, ibo; // IBO stands for "index buffer object"
      uint indices;
};

#endif
