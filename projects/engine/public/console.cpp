#include <console.hpp>

#include <stdio.h>
#include <string.h>
#include <regex>

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

   // Ugly but strict on purpose
   if (i == 0)
      return false;
   else if (i == 1)
      return 1;

   // Same case here
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
	 printf("Successfully created and registered ConFunc '%s'\n", signature.c_str());
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
	 printf("Successfully created and registered ConVar '%s'\n", signature.c_str());
      else { 
	 printf("Failed to register ConVar '%s'\n", signature.c_str());
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

void ProcessObject(ConObject* object, std::vector<std::string> args) {
   if (object != nullptr) {
      object->Execute(args);
   }
}

void ConsoleInstance::ParseCommandLine(int argc, char **argv) {
   // - is cfunc
   // + is cvar
   std::vector<std::string> args;
   ConObject* object = nullptr;

   for (int a = 0; a < argc; a++) {
      if (argv[a][0] == '-' || argv[a][0] == '+') {
	 ProcessObject(object, args);
	 object = nullptr;
	 args.clear();

	 std::string arg = argv[a];
	 arg = arg.substr(1);

	 if (objects.count(arg) > 0) {
	    object = objects[arg]; 
	 }
      } else {
	 if (object != nullptr)
	    args.push_back(argv[a]);
      }
   }

   ProcessObject(object, args);
   args.clear();
}

ConVar* ConsoleInstance::GetCVar(std::string signature) {
   if (objects.count(signature) > 0) {
      ConObject* object = objects[signature];

      if (object != nullptr) {
	 ConVar* cvar = dynamic_cast<ConVar*>(object);
	 return cvar;
      }
   }

   return nullptr;
}

bool ConsoleInstance::canCreateConObject(std::string signature) {
   return objects.count(signature) == 0;
}
