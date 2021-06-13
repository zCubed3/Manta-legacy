#include <console.hpp>

#include <stdio.h>
#include <string.h>

#include <regex>
#include <iostream>
#include <fstream>

ConObject::ConObject(std::string signature) {
   this->signature = signature;
}

//
// ConFunc (CFunc)
//
ConFunc::ConFunc(std::string signature, ConFuncBinding func) : ConObject(signature) { function = func; } 

void ConFunc::Execute(std::vector<std::string> args) {
   if (function != nullptr) {
      function(args);
   }
}

//
// ConVar (CVar)
//
ConVar::ConVar(std::string signature, std::string data) : ConObject(signature) { this->data = data; }

void ConVar::Execute(std::vector<std::string> args) {
   // CVars are only set to the first argument
   if (args.size() > 0)
      data = args[0];
   else
      data = "";

   printf("Set cvar %s to %s\n", signature.c_str(), data.c_str());
}

int ConVar::ParseInt(int default_) {
   std::regex digitRegex("^\\d+$");

   if (std::regex_match(data, digitRegex)) {
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
ConsoleInstance::~ConsoleInstance() {
   for (auto o = objects.begin(); o != objects.end(); o++) {
      if (o->second == nullptr)
	 continue;

      printf("Cleaning up ConObject %s\n", o->first.c_str());
      delete o->second;
   }
}

void ConsoleInstance::CreateCFunc(std::string signature, ConFuncBinding func) {
   if (canCreateConObject(signature))
   { 
      ConFunc* cfunc = new ConFunc(signature, func);

      if (RegisterObject(cfunc))
	 printf("Registered CFunc '%s'\n", signature.c_str());
      else { 
	 printf("Failed to register ConFunc '%s'\n", signature.c_str());
	 delete cfunc;
      }
   }
}

void ConsoleInstance::CreateCVar(std::string signature, std::string data) {
   if (canCreateConObject(signature))
   { 
      ConVar* cvar = new ConVar(signature, data);

      if (RegisterObject(cvar))
	 printf("Registered CVar '%s'\n", signature.c_str());
      else { 
	 printf("Failed to register CVar'%s'\n", signature.c_str());
	 delete cvar;
      }
   }
}

bool ConsoleInstance::RegisterObject(ConObject* object) { 
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

void ProcessObject(ConObject** object, std::vector<std::string>* args) {
   if (object != nullptr && *object != nullptr && args != nullptr) {
      (*object)->Execute(*args);
      args->clear();

      // This isn't very good code but it helps keep clutter down
   }
}

void ConsoleInstance::ParseAutoExec() {
   // Parsed a bit differently to the command line, does it line by line instead of arg by arg

   std::ifstream autoExec("./data/autoexec");

   if (!autoExec.is_open()) {
      printf("Failed to open autoexec, was expecting it to be in ./data/autoexec, is it missing or moved?\n");
      return;
   }

   std::vector<std::string> args;
   std::string line;

   printf("Parsing commands from autoexec:\n\n");
   while (std::getline(autoExec, line)) {
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
	    ProcessObject(&objects[cmd], &args);
	 else
	    printf("Unknown commmand %s\n", cmdRaw.c_str());
      }
   }

   printf("\nDone parsing from autoexec!\n");
}

void ConsoleInstance::ParseCommandLine(int argc, char **argv) {
   // - is cfunc
   // + is cvar
   std::vector<std::string> args;
   ConObject** object = nullptr;

   printf("Parsing commands from command line:\n\n");

   for (int a = 1; a < argc; a++) {
      if (argv[a][0] == '-' || argv[a][0] == '+') {
	 ProcessObject(object, &args);
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

   ProcessObject(object, &args);

   printf("\nDone parsing from command line!\n");
}

ConVar* ConsoleInstance::GetCVar(std::string signature) {
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
      printf("CVar '%s' doesn't exist!\n", signature.c_str());

   return nullptr;
}

bool ConsoleInstance::canCreateConObject(std::string signature) {
   return objects.count(signature) == 0;
}

// Quick CVar getters

int ConsoleInstance::CVarGetInt(std::string signature, int default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->ParseInt(default_);

   return default_;
}

bool ConsoleInstance::CVarGetBool(std::string signature, bool default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->ParseBool(default_);

   return default_;
}

std::string ConsoleInstance::CVarGetData(std::string signature, std::string default_) {
   ConVar* cvar = GetCVar(signature);

   if (cvar)
      return cvar->data;

   return default_;
}

