#include "common_console.hpp"

#include <console.hpp>

#include <stdio.h>

bool shouldQuit = false;
void setQuit(std::vector<std::string> args) { shouldQuit = true; }

void CreateCommonConObjects(Console* console) {
   if (!console) {
      printf("Could not create common ConObjects, the given console points to nullptr!\n");
      return;
   }

   //
   // Common cvars
   //

   console->CreateCVar("gamename", "Manta Game");
   console->CreateCVar("fov", "90"); // Can be ignored by cameras

   //
   // Common cfuncs
   //
   
   console->CreateCFunc("quit", &setQuit);
}
