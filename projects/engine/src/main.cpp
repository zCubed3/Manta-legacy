#include <dynamiclib.hpp>
#include <renderer.hpp>

#include <manta_macros.hpp>
#include <manta_errcodes.hpp>
#include <console.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>

void ConTestFunction(std::vector<std::string> args) {
  printf("ConTestFunc()!\n");

  for (int a = 0; a < args.size(); a++)
     printf("%s\n", args[a].c_str());
}

int main(int argc, char** argv) {
   ConsoleInstance console;

   console.CreateCFunc("test_cmd", &ConTestFunction);
   console.CreateCVar("test_cvar", "test");   

   console.CreateCVar("gamename", "Manta");

   DynamicLib* rendererLib = LoadDynamicLib("./lib/OpenGL3_api");

   if (rendererLib) {
      FuncGetRenderer funcGetRenderer = (FuncGetRenderer)rendererLib->GetFunction("get_Renderer");
      Renderer* renderer = nullptr;

      if (funcGetRenderer != nullptr)
	 renderer = funcGetRenderer();

      if (renderer == nullptr) {
	 printf("Failed to create renderer\n");
	 return ERR_FAILED_TO_CREATE_RENDERER;
      }

      renderer->console = &console;
      renderer->RegisterConObjects();
   
      console.ParseCommandLine(argc, argv);

      renderer->Initialize();

      Renderer::RendererState state;
      while (true) {
	 state = renderer->Render();

	 // TODO: Make this be more verbose
	 if (state.status != Renderer::Status::Success)
	    break;

      }
   }
}

void HandleCmd(int argc, char** argv) {
   for (int a = 0; a < argc; a++) {
      char* arg = argv[a];

      if (arg == NULL) // Just in case
	 continue;

      if (strlen(arg) == 0) // Also just in case we get fed bad info from the OS
	 continue;

      // Like Source and Id Tech
      // - is command
      // + is convar
      if (arg[0] == '-') {
      	 char* cmd = new char[strlen(arg) - 1]; // Trim the first character
	 strcpy(cmd, arg + 1);

	 printf("Parsing CMD %s\n", cmd);

	 delete[] cmd;
      }
   }
}
