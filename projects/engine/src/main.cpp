#include <dynamiclib.hpp>
#include <renderer.hpp>

#include <stdlib.h>
#include <string.h>

#include <iostream>

void HandleCmd(int argc, char** argv);

int main(int argc, char** argv) {
   HandleCmd(argc, argv);
   
   DynamicLib* rendererLib = LoadDynamicLib("lib/opengl_api.so");

   if (rendererLib) {
      void* func = rendererLib->GetFunction("get_Renderer");
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
