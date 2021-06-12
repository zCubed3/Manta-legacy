#include <opengl3_api.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <console.hpp>

#include <assets/glvertexbuffer.hpp>
#include <assets/glshaderprogram.hpp>

//
// Callbacks
//

// TODO: Make this set a permanent variable so RenderTargets are possible
void OnFramebufferResize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height); 
}

//
// Renderer functionality
//

void GL3Renderer::Initialize() {
   glewExperimental = true; // Enables more features

   if (glfwInit()) {
      printf("Initialized GLFW\n");

      bool isResizable = false;
      int winWidth = 800;
      int winHeight = 600;
      const char* winTitle = "Manta";

      if (console) {
	 ConVar* cvar = console->GetCVar("r_window_resizable");

	 if (cvar) {
	    isResizable = cvar->ParseBool(false);
	    printf("Set window resizability to %s\n", (isResizable ? "true" : "false"));      
	    cvar = nullptr;
	 }

	 cvar = console->GetCVar("r_window_width");

	 if (cvar) {
	    winWidth = cvar->ParseInt(800);
	    printf("Set window width to %i\n", winWidth);
	    cvar = nullptr;
	 }

	 cvar = console->GetCVar("r_window_height");

	 if (cvar) {
	    winHeight = cvar->ParseInt(600);
	    printf("Set window height to %i\n", winHeight);
	    cvar = nullptr;
	 }

	 cvar = console->GetCVar("gamename");

	 if (cvar) {
	    winTitle = cvar->data.c_str();
	    cvar = nullptr;
	 }
      }

      glfwWindowHint(GLFW_RESIZABLE, isResizable);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

      window = glfwCreateWindow(winWidth, winHeight, winTitle, nullptr, nullptr);
      glfwMakeContextCurrent(window);

      glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

      if (glewInit() != GLEW_OK) {
	 printf("Failed to initialze GLEW!\n");
	 return;
      }

      shaderLoader.LoadCode("default", "");
   } else {
      printf("Failed to initialize GLFW\n");
   }
}

Renderer::RendererState GL3Renderer::Render() {
   RendererState state;

   state.status = Renderer::Status::Success;

   glfwPollEvents();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

   for (int m = 0; m < modelQueue.size(); m++)
      if (modelQueue[m])
	 modelQueue[m]->Draw();

   if (glfwWindowShouldClose(window))
      state.status = Renderer::Status::ShutDown;

   glfwSwapBuffers(window);

   //printf("GL Error: %i\n", glGetError());

   return state;
}

void GL3Renderer::CreateBuffer(Model* model) {
   if (model != nullptr) {
      if (model->vertexBuffer) {
	 printf("Model already has a vertex buffer, deleting it!\n");
	 delete model->vertexBuffer;
      }

      model->vertexBuffer = new GL3VertexBuffer();
      model->vertexBuffer->Populate(model);
   }
}

void GL3Renderer::CreateShaderProgram(Shader *shader) {
   if (shader != nullptr) {
      if (shader->program != nullptr) {
	 printf("Shader already has a program, deleting it!\n");
	 delete shader->program;
      }

      shader->program = new GL3ShaderProgram();
      shader->program->Compile(shader); 
   }
}
