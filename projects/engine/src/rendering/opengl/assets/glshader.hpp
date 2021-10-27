#ifndef MANTAGL3_GLSHADER_H
#define MANTAGL3_GLSHADER_H

#include <unordered_map>
#include <assets/shader.hpp>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

typedef unsigned int uint;

class GLShaderProgram : public ShaderProgram {
public:
    bool Compile(Shader *shader) override;

    void Bind() override;

    ~GLShaderProgram() override;

    std::unordered_map<std::string, int> uniforms;

    int getUniform(const std::string &name);

    void setInt(const std::string &name, int value);

    void setFloat(const std::string &name, float value);

    void setVec3(const std::string &name, glm::vec3 vec3);

    void setVec4(const std::string &name, glm::vec4 vec4); // aka setColor

    void setMat4x4(const std::string &name, glm::mat4x4 matrix, bool transpose = false);

    uint program;
};

#endif
