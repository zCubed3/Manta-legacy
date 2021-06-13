#include "common_console.hpp"

#include <console.hpp>

#include <stdio.h>

bool shouldQuit = false;
void setQuit(std::vector<std::string> args) { shouldQuit = true; }

void CreateCommonConObjects(ConsoleInstance* console) {
   if (!console) {
      printf("Could not create common ConObjects, the given console points to nullptr!\n");
      return;
   }

   //
   // Common cvars
   //

   console->CreateCVar("gamename", "Manta Game");
   console->CreateCVar("manta_debug", "0");

   //
   // Common cfuncs
   //
   
   console->CreateCFunc("quit", &setQuit);
}
