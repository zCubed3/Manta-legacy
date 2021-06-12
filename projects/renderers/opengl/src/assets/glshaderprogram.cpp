#include <assets/glshaderprogram.hpp>

#include <stdio.h>

#include <GL/glew.h>

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

   const char* vSources[2] = {"#version 330 core\n#define VERTEX\n", shader->code.c_str()};
   glShaderSource(vShader, 2, vSources, nullptr);

   const char* fSources[2] = {"#version 330 core\n#define FRAGMENT\n", shader->code.c_str()};
   glShaderSource(fShader, 2, fSources, nullptr);

   glCompileShader(vShader);
   glCompileShader(fShader);

   bool successful = true;

   if (didCompileShader(vShader) && didCompileShader(fShader)) {
      uint tempProgram = glCreateProgram(); 
      
      glAttachShader(program, vShader);
      glAttachShader(program, fShader);

      glLinkProgram(program);

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
