#ifndef MANTA_GLMODEL_H
#define MANTA_GLMODEL_H

#include <assets/model.hpp>

typedef unsigned int uint;

class GLVertexBuffer : public VertexBuffer {
public:
    void Populate(Model *model);

    void Draw(Renderer *renderer, AActor *pActor, Shader *shader);

    ~GLVertexBuffer();

protected:
    uint vao, vbo, ibo; // IBO stands for "index buffer object"
    uint indices;
};

#endif
