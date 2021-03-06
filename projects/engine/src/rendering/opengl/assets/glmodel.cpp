#include "glmodel.hpp"
#include "glshader.hpp"

#include <GL/glew.h>

#include <assets/shader.hpp>
#include <rendering/renderer.hpp>

#include <actors/world.hpp>

#include <glm/glm/gtc/type_ptr.hpp>

#include <rendering/opengl/glrenderer.hpp>
#include <string>

#include <assets/material.hpp>

#include <core/engine.hpp>

GLVertexBuffer::~GLVertexBuffer() {
    if (vao != GL_INVALID_INDEX)
        glDeleteVertexArrays(1, &vao);

    if (vbo != GL_INVALID_INDEX)
        glDeleteBuffers(1, &vbo);

    if (ibo != GL_INVALID_INDEX)
        glDeleteBuffers(1, &ibo);
}

void GLVertexBuffer::Populate(Model *model) {
    printf("Creating OpenGL3 VBO for %s\n", model->name.c_str());

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex) * model->vertices.size(), model->vertices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * model->triangles.size(), model->triangles.data(),
                 GL_STATIC_DRAW);

    indices = model->triangles.size();

    glBindVertexArray(0);
}

void GLVertexBuffer::Draw(MEngine *engine, AActor *pActor, Material *material) {
    if (material) {
        material->Bind();
    } else {
        return;
    }

    GLShaderProgram *pProgram = dynamic_cast<GLShaderProgram *>(material->pShader->program);
    GLRenderer *glRenderer = dynamic_cast<GLRenderer *>(engine->renderer);

    if (pProgram) {
        uint program = pProgram->program;
        int location;

        pProgram->setInt("MANTA_GBUFFER_POS", 0);
        pProgram->setInt("MANTA_GBUFFER_NORMAL", 1);
        pProgram->setInt("MANTA_GBUFFER_ALBEDO", 2);
        pProgram->setInt("MANTA_GBUFFER_MRA", 3);
        pProgram->setInt("MANTA_GBUFFER_EMISSION", 4);

        pProgram->setInt("MANTA_CUBEMAP_ENVIRONMENT", 5);

        pProgram->setInt("MANTA_TEX_BRDF_LUT", 6);

        if (pActor) {
            pProgram->setMat4x4("MANTA_MATRIX_M", pActor->mModel);
            pProgram->setMat4x4("MANTA_MATRIX_M_IT", pActor->mModel_it);
        }

        if (engine->world.pCamera) {
            pProgram->setMat4x4("MANTA_MATRIX_V", engine->world.pCamera->mView);
            pProgram->setMat4x4("MANTA_MATRIX_P", engine->world.pCamera->mPerspective);
            pProgram->setVec3("MANTA_CAMERA_POSITION", engine->world.pCamera->position);
        }

        pProgram->setInt("MANTA_LIGHT_COUNT", engine->world.data.lightCount);

        pProgram->setVec3("MANTA_AMBIENT_COLOR", engine->world.data.ambientColor);

        char locationString[64];

        for (int l = 0; l < engine->world.data.lightCount; l++) {
            sprintf(locationString, "MANTA_LIGHT_POSITIONS[%i]", l);
            pProgram->setVec3(locationString, engine->world.data.lightPositions[l]);

            sprintf(locationString, "MANTA_LIGHT_DIRECTIONS[%i]", l);
            pProgram->setVec3(locationString, engine->world.data.lightDirections[l]);

            sprintf(locationString, "MANTA_LIGHT_COLORS[%i]", l);
            pProgram->setVec3(locationString, engine->world.data.lightColors[l]);

            sprintf(locationString, "MANTA_LIGHT_RANGES[%i]", l);
            pProgram->setFloat(locationString, engine->world.data.lightRanges[l]);

            sprintf(locationString, "MANTA_LIGHT_INTENSITIES[%i]", l);
            pProgram->setFloat(locationString, engine->world.data.lightIntensities[l]);

            sprintf(locationString, "MANTA_LIGHT_PARAMS1[%i]", l);
            pProgram->setFloat(locationString, engine->world.data.lightParams1[l]);

            sprintf(locationString, "MANTA_LIGHT_PARAMS2[%i]", l);
            pProgram->setFloat(locationString, engine->world.data.lightParams2[l]);

            sprintf(locationString, "MANTA_LIGHT_TYPES[%i]", l);
            pProgram->setInt(locationString, engine->world.data.lightTypes[l]);
        }

        glUniform1f(glGetUniformLocation(program, "MANTA_TIME"), engine->world.timeTotal);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    int vertSize = sizeof(Model::Vertex);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertSize, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertSize, (void *) (sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertSize, (void *) (sizeof(float) * 6));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindVertexArray(0);
}
