#include <gl3_renderer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <console.hpp>

#include <assets/glvertexbuffer.hpp>
#include <assets/glshaderprogram.hpp>

#include <entities/light.hpp>
#include <entities/world.hpp>

//
// Callbacks
//

// TODO: Make this set a permanent variable so RenderTargets are possible
void OnFramebufferResize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);

   void* rendererRaw = glfwGetWindowUserPointer(window);
   if (!rendererRaw)
      return;

   Renderer* renderer = (Renderer*)rendererRaw;

   if (renderer) {
      renderer->windowWidth = width;
      renderer->windowHeight = height;
   }
}

//
// Renderer functionality
//

void GL3Renderer::Initialize() {
   glewExperimental = true; // Enables more features

   if (glfwInit()) {
      printf("Initialized GLFW\n");

      bool isResizable = false;
      bool fullscreen = false;
      int winWidth = 800;
      int winHeight = 600;
      std::string winTitle = "Manta";
      int sampleCount = 1;
      bool transparentFB = false;

      if (console) {
	 isResizable = console->CVarGetBool("r_window_resizable", false);
	 winWidth = console->CVarGetInt("width", 800);
	 winHeight = console->CVarGetInt("height", 600);
	 winTitle = console->CVarGetData("gamename", "GL3Window");
	 fullscreen = console->CVarGetBool("fullscreen", false);
	 
	 sampleCount = console->CVarGetInt("gl_samples", 1);
      }

      glfwWindowHint(GLFW_RESIZABLE, isResizable);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_SAMPLES, sampleCount);

      GLFWmonitor* monitor = nullptr;

      if (fullscreen)
	 monitor = glfwGetPrimaryMonitor();

      window = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), monitor, nullptr);
      glfwMakeContextCurrent(window);
      
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      windowWidth = winWidth;
      windowHeight = winHeight;

      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

      if (glewInit() != GLEW_OK) {
	 printf("Failed to initialze GLEW!\n");
	 return;
      }

      shaderLoader.LoadCode("engine#error", "");
      shaderLoader.LoadCode("engine#default", "");
      
      glGenBuffers(1, &worldUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, worldUBO);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(WorldData), nullptr, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, 0, worldUBO);
   }
   else {
      printf("Failed to initialize GLFW\n");
   }
}

Renderer::RendererState GL3Renderer::Render() {
   RendererState state;

   state.status = Renderer::Status::Success;

   glfwPollEvents();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

   timeTotal = glfwGetTime();
   timeDelta = timeTotal - timeLast;
   timeLast = timeTotal;

   if (world) {
      glBindBuffer(GL_UNIFORM_BUFFER, worldUBO);
      void* worldBuffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
      memcpy(worldBuffer, &world->data, sizeof(WorldData));
      glUnmapBuffer(GL_UNIFORM_BUFFER);

      for (int e = 0; e < world->entities.size(); e++) {
	 if (world->entities[e] == nullptr)
	    continue;

	 world->entities[e]->Draw(this);
      }
   }

   glfwSwapBuffers(window);
   if (glfwWindowShouldClose(window)) {
      state.status = Renderer::Status::ShutDown;
      glfwDestroyWindow(window);
   }

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

void GL3Renderer::RegisterConObjects() {
   Renderer::RegisterConObjects();

   if (console) {
      console->CreateCVar("gl_samples", "1");
      
   }
}
