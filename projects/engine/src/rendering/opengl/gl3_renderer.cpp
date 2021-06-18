#include "gl3_renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <console/console.hpp>

#include "assets/glmodel.hpp"
#include "assets/glshader.hpp"

#include <entities/light.hpp>
#include <entities/world.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <assets/resources.hpp>

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

// Shadowmaps based on https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
void GL3Renderer::Initialize() {
   glewExperimental = true; // Enables more features

   if (glfwInit()) {
      printf("Initialized GLFW\n");
 
      glfwWindowHint(GLFW_RESIZABLE, console->CVarGetBool("r_window_resizable", false));
      glfwWindowHint(GLFW_SAMPLES, console->CVarGetInt("r_msaa_samples", 1));
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      
      GLFWmonitor* monitor = nullptr;

      if (console->CVarGetBool("fullscreen", false))
	 monitor = glfwGetPrimaryMonitor();

      window = glfwCreateWindow(console->CVarGetInt("width", 800), console->CVarGetInt("height", 600), console->CVarGetData("gamename", "Manta").c_str(), monitor, nullptr);
      glfwMakeContextCurrent(window);

      // Supply the renderer some info
      int intWidth, intHeight;
      glfwGetWindowSize(window, &intWidth, &intHeight); 
      windowWidth = intWidth;
      windowHeight = intHeight;

      // Depth testing
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      // Backface culling
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

      if (glewInit() != GLEW_OK) {
	 printf("Failed to initialze GLEW!\n");
	 return;
      }

      // Internal engine shaders
      CreateShaderProgram(resources->shaderLoader.LoadCode("engine#error", gl3ErrorShaderCode));
   }
   else {
      printf("Failed to initialize GLFW\n");
   }
}

void GL3Renderer::BeginRenderShadow() {
   int originalX = windowWidth, originalY = windowHeight;
   glViewport(0, 0, shadowmapWidth, shadowmapHeight);
   glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO);
   glClear(GL_DEPTH_BUFFER_BIT);
}

void GL3Renderer::EndRenderShadow() {
   int originalX = windowWidth, originalY = windowHeight;
   glViewport(0, 0, originalX, originalY);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GL3Renderer::BeginRender() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

   if (vsync)
      glfwSwapInterval(vsync);
}

Renderer::Status GL3Renderer::EndRender() {
   Renderer::Status state = Renderer::Status::Running;

   glfwSwapBuffers(window);
   if (glfwWindowShouldClose(window)) {
      state = Renderer::Status::ShuttingDown;
      glfwDestroyWindow(window);
   }

   return state;

}

void GL3Renderer::InitImGui() {
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 150");
}

void GL3Renderer::BeginImGui() {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame(); 
}

void GL3Renderer::EndImGui() {
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GL3Renderer::CreateVertexBuffer(Model* model) {
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

void GL3Renderer::CreateConObjects(Console* console) {
   Renderer::CreateConObjects(console);

   if (console) {
      //console->CreateCVar("gl_samples", "1");
      
   }
}

void GL3Renderer::DrawImGuiWindow() {
   if (!showImGuiWindow)
      return;

   ImGui::Begin("Renderer");
   
   ImGui::Text("OpenGL 3.3");
   ImGui::Checkbox("VSync", &vsync);

   ImGui::End();
}

//
// Built-in shaders
//

std::string gl3ErrorShaderCode = R"(
#ifdef VERTEX
layout(location = 0) in vec3 _vertex;

uniform mat4 MANTA_mM;
uniform mat4 MANTA_mV;
uniform mat4 MANTA_mP;

void main() {
  mat4 mMVP = MANTA_mP * MANTA_mV * MANTA_mM;
  gl_Position = mMVP * vec4(_vertex, 1.0);
}

#endif

#ifdef FRAGMENT

uniform float MANTA_fTime;
out vec3 color;

void main() {
   // Color is #ff7300
   color = vec3(1.0, 0.45098039215, 0.0) * abs(sin(MANTA_fTime * 2));
}

#endif
)";
