#include "glshader.hpp"

#include <stdio.h>

#include <rendering/renderer.hpp>
#include <GL/glew.h>

GL3ShaderProgram::~GL3ShaderProgram() {
   if (program != 0) {
      glDeleteProgram(program);
   }
}

void GL3ShaderProgram::Bind() {
   glUseProgram(program);
}

bool didCompileShader(uint shader) {
   int status;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

   if (status != GL_TRUE) {
      int logLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

      char* log = new char[logLen];

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

      char* log = new char[logLen];

      glGetProgramInfoLog(program, logLen, nullptr, log);
      printf("Program failed to compile with error:\n%s\n", log);

      delete[] log;
   }

   return status;
}


// TODO Geometry shaders?
void GL3ShaderProgram::Compile(Shader *shader) {
   printf("Beginning compilation of GLSL program\n");

   uint vShader = glCreateShader(GL_VERTEX_SHADER);
   uint fShader = glCreateShader(GL_FRAGMENT_SHADER);   

   const char* vSources[2] = {"#version 330\n#define VERTEX\n", shader->code.c_str()};
   glShaderSource(vShader, 2, vSources, nullptr);

   const char* fSources[2] = {"#version 330\n#define FRAGMENT\n", shader->code.c_str()};
   glShaderSource(fShader, 2, fSources, nullptr);

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
   }
   else
      successful = false;

   glDeleteShader(vShader);
   glDeleteShader(fShader);

   if (successful)
      printf("Done!\n");
   else
      printf("Failed...\n");
}

int GL3ShaderProgram::getUniform(std::string name) {
   if (uniforms.count(name) >= 0)
      return uniforms[name];

   int uniform = glGetUniformLocation(program, name.c_str());
   uniforms.emplace(name, uniform);
   return uniform;
}
