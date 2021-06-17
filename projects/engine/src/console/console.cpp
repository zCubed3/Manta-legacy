#include "console.hpp"

#include <stdio.h>
#include <string.h>

#include <regex>
#include <iostream>
#include <fstream>

ConObject::ConObject(std::string signature, bool isProtected) {
   this->signature = signature;
   this->isProtected = isProtected;
}

//
// ConFunc (CFunc)
//
ConFunc::ConFunc(std::string signature, ConFuncBinding func, bool isProtected) : ConObject(signature, isProtected) { function = func; } 

void ConFunc::Execute(Console* console, std::vector<std::string> args) {
   if (isProtected && !console->protectionDisabled) {
      printf("Failed to execute %s, please run -e_disable_protection to use this function", signature.c_str());
      return;
   }

   if (function != nullptr)
      function(console, args);
}

//
// ConVar (CVar)
//
ConVar::ConVar(std::string signature, std::string data, bool isProtected) : ConObject(signature, isProtected) { this->data = data; }

void ConVar::Execute(Console* console, std::vector<std::string> args) {
   // CVars are only set to the first argument
   if (isProtected && !console->protectionDisabled) {
      printf("Failed to set %s, please run -e_disable_protection to use this variable", signature.c_str());
      return;
   }

   if (args.size() > 0)
      data = args[0];
   else
      data = "";

   printf("Set cvar %s to %s\n", signature.c_str(), data.c_str());
}

int ConVar::ParseInt(int default_) {
   if (std::all_of(data.begin(), data.end(), ::isdigit)) {
      int number = default_; 
      sscanf(data.c_str(), "%i", &number);

      return number;
   }

   return default_;
}

void ConVar::ParseInt(int* data, int default_) { *data = ParseInt(default_); }

bool ConVar::ParseBool(bool default_) {
   int i = -1;
   sscanf(data.c_str(), "%i", &i);

   if (i < 2 && i >= 0)
      return i;

   // Strict and ugly
   std::regex trueRegex("[Tt]rue");
   std::regex falseRegex("[Ff]alse");

   if (std::regex_match(data, trueRegex))
      return true;
   else if (std::regex_match(data, falseRegex))
      return false;

   return default_;
}

void ConVar::ParseBool(bool* data, bool default_) { *data = ParseBool(default_); }

//
// ConsoleInstance
//
Console::~Console() {
   printf("Cleaning up Console...\n");
   for (auto o = objects.begin(); o != objects.end(); o++) {
      if (o->second == nullptr)
	 continue;

      printf("Cleaning up ConObject %s\n", o->first.c_str());
      delete o->second;
   }
}

void Console::CreateCFunc(std::string signature, ConFuncBinding func, bool isProtected) {
   if (canCreateConObject(signature))
   { 
      ConFunc* cfunc = new ConFunc(signature, func, isProtected);

      if (RegisterObject(cfunc))
	 printf("Registered CFunc '%s'\n", signature.c_str());
      else { 
	 printf("Failed to register ConFunc '%s'\n", signature.c_str());
	 delete cfunc;
      }
   }
}

void Console::CreateCVar(std::string signature, std::string data, bool isProtected) {
   if (canCreateConObject(signature))
   { 
      ConVar* cvar = new ConVar(signature, data, isProtected);

      if (RegisterObject(cvar))
	 printf("Registered CVar '%s'\n", signature.c_str());
      else { 
	 printf("Failed to register CVar'%s'\n", signature.c_str());
	 delete cvar;
      }
   }
}

bool Console::RegisterObject(ConObject* object) { 
   if (object == nullptr) {
      printf("Given ConObject was a nullptr!\n");
      return false;
   }
   
   int found = objects.count(object->signature);

   if (found > 0) {
      printf("An object is already registered with a signature of %s\n", object->signature.c_str());
      return false;
   }
   else {
      objects.emplace(object->signature, object);
      return true;
   }
}

void ProcessObject(ConObject** object, Console* console, std::vector<std::string>* args) {
   if (object != nullptr && *object != nullptr && args != nullptr && console != nullptr) {
      (*object)->Execute(console, *args);
      args->clear();

      // This isn't very good code but it helps keep clutter down
   }
}

void Console::ParseExecFile(std::string path) {
   // Parsed a bit differently to the command line, does it line by line instead of arg by arg

   std::ifstream execFile(path);

   if (!execFile.is_open()) {
      printf("Failed to open file for execution, %s\n", path.c_str());
      return;
   }

   std::vector<std::string> args;
   std::string line;

   printf("Parsing commands from %s:\n\n", path.c_str());
   while (std::getline(execFile, line)) {
      if (line.size() <= 0 || line[0] == '#')
	 continue;

      if (line[0] == '-' || line[0] == '+') {
	 char* token = strtok(line.data(), " ");
	 std::string cmdRaw = token; // Token 0 is command, following are args
	 token = strtok(nullptr, " ");

	 // Strip the first char of cmd
	 std::string cmd = cmdRaw.substr(1);

	 while (token != nullptr) {
	    args.push_back(token);
	    token = strtok(nullptr, " ");
	 }

	 if (objects.count(cmd) > 0)
	    ProcessObject(&objects[cmd], this, &args);
	 else
	    printf("Unknown commmand %s\n", cmdRaw.c_str());
      }
   }

   printf("\nDone parsing from file!\n");
}

void Console::ParseCommandLine(int argc, char **argv) {
   // - is cfunc
   // + is cvar
   std::vector<std::string> args;
   ConObject** object = nullptr;

   printf("Parsing commands from command line:\n\n");

   for (int a = 1; a < argc; a++) {
      if (argv[a][0] == '-' || argv[a][0] == '+') {
	 ProcessObject(object, this, &args);
	 object = nullptr;

	 std::string arg = argv[a];
	 arg = arg.substr(1);

	 if (objects.count(arg) > 0)
	    object = &objects[arg];
	 else
	    printf("Unknown command %s\n", argv[a]);
      } else {
	 if (object != nullptr)
	    args.push_back(argv[a]);
      }
   }

   ProcessObject(object, this, &args);

   printf("\nDone parsing from command line!\n");
}

ConVar* Console::GetCVar(std::string signature, bool silent) {
   if (objects.count(signature) > 0) {
      ConObject* object = objects[signature];

      //printf("Listing CVars\n");
      //for (auto o = objects.begin(); o != objects.end(); o++)
	 //printf("CVar: %s:%i\n", o->first.c_str(), o->second == nullptr);

      if (object != nullptr) {
	 ConVar* cvar = dynamic_cast<ConVar*>(object);
	 return cvar;
      }
   }
   else
      if (!silent)
	 printf("CVar '%s' doesn't exist!\n", signature.c_str());

   return nullptr;
}

bool Console::canCreateConObject(std::string signature) {
   return objects.count(signature) == 0;
}

// Quick CVar getters

int Console::CVarGetInt(std::string signature, int default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->ParseInt(default_);

   return default_;
}

bool Console::CVarGetBool(std::string signature, bool default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->ParseBool(default_);

   return default_;
}

std::string Console::CVarGetData(std::string signature, std::string default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->data;

   return default_;
}

