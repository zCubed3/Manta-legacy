#include "glshader.hpp"

#include <iostream>
#include <exception>

#include <rendering/renderer.hpp>
#include <GL/glew.h>

GLShaderProgram::~GLShaderProgram() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

void GLShaderProgram::Bind() {
    glUseProgram(program);
}

bool didCompileShader(uint shader) {
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        int logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        char *log = new char[logLen];

        glGetShaderInfoLog(shader, logLen, nullptr, log);
        printf("Shader failed to compile with error:\n%s\n", log);

        delete[] log;
    }

    return status;
}

bool didCompileProgram(uint program) {
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
        int logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        char *log = new char[logLen];

        glGetProgramInfoLog(program, logLen, nullptr, log);
        printf("Program failed to compile with error:\n%s\n", log);

        delete[] log;
    }

    return status;
}


// TODO Geometry shaders?
bool GLShaderProgram::Compile(Shader *shader) {
    printf("Beginning compilation of GLSL program\n");

    uint vShader = glCreateShader(GL_VERTEX_SHADER);
    uint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    // TODO Make this better?
    // Shitty checking but we remove the first line and place it into another string
    auto firstLine = shader->code.find_first_of('\n');
    std::string version = shader->code.substr(0, firstLine) + "\n";

    // Then trim it from the actual code
    shader->code = shader->code.substr(firstLine);

    const char *vSources[3] = {version.c_str(), "#define VERTEX\n", shader->code.c_str()};
    glShaderSource(vShader, 3, vSources, nullptr);

    const char *fSources[3] = {version.c_str(), "#define FRAGMENT\n", shader->code.c_str()};
    glShaderSource(fShader, 3, fSources, nullptr);

    bool vValid = false;
    glCompileShader(vShader);
    vValid = didCompileShader(vShader);

    bool fValid = false;
    glCompileShader(fShader);
    fValid = didCompileShader(fShader);

    bool successful = true;

    if (vValid && fValid) {
        uint tempProgram = glCreateProgram();

        glAttachShader(tempProgram, vShader);
        glAttachShader(tempProgram, fShader);

        glLinkProgram(tempProgram);

        if (didCompileProgram(tempProgram))
            program = tempProgram;
        else
            successful = false;

        glDetachShader(program, vShader);
        glDetachShader(program, fShader);
    } else
        successful = false;

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    if (successful)
        printf("Done!\n");
    else
        printf("Failed...\n");

    return successful;
}

int GLShaderProgram::getUniform(const std::string &name) {
    if (uniforms.count(name) > 0) // We cache the uniforms, this might be slow though
        return uniforms[name];

    int uniform = glGetUniformLocation(program, name.c_str());

    if (uniform == -1) {
        std::cerr << "Couldn't find a uniform with the signature '" << name << "'\n";
        //throw std::runtime_error("Failed to find uniform!");
    }

    uniforms.emplace(name, uniform);
    return uniform;
}

void GLShaderProgram::setInt(const std::string &name, int value) {
    try {
        glUniform1i(getUniform(name), value);
    }
    catch (std::exception &e) {
        std::cerr << "Caught an exception trying to set an integer!\n" << e.what() << "\n";
    }
}

void GLShaderProgram::setFloat(const std::string &name, float value) {
    try {
        glUniform1f(getUniform(name), value);
    }
    catch (std::exception &e) {
        std::cerr << "Caught an exception trying to set a float!\n" << e.what() << "\n";
    }
}

void GLShaderProgram::setVec3(const std::string &name, glm::vec3 vec3) {
    try {
        glUniform3fv(getUniform(name), 1, glm::value_ptr(vec3));
    }
    catch (std::exception &e) {
        std::cerr << "Caught an exception trying to set a vec3!\n" << e.what() << "\n";
    }
}

void GLShaderProgram::setVec4(const std::string &name, glm::vec4 vec4) {
    try {
        glUniform4fv(getUniform(name), 1, glm::value_ptr(vec4));
    }
    catch (std::exception &e) {
        std::cerr << "Caught an exception trying to set a vec4!\n" << e.what() << "\n";
    }
}

void GLShaderProgram::setMat4x4(const std::string &name, glm::mat4x4 matrix, bool transpose) {
    try {
        glUniformMatrix4fv(getUniform(name), 1, transpose, glm::value_ptr(matrix));
    }
    catch (std::exception &e) {
        std::cerr << "Caught an exception trying to set a mat4x4!\n" << e.what() << "\n";
    }
}
