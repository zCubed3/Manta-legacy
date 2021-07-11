#include "monoruntime.hpp"
#include "mono/metadata/assembly.h"

#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/debug-helpers.h>

#include <manta_macros.hpp>

#include <filesystem>
using namespace std::filesystem;

// Useless normally, only here so I don't throw up over repeated code
void mono_printf(const char* text) {
   printf("[Mono]: %s", text);
}

void MonoScriptingBackend::Initialize() {
   mono_printf("Initializing Mono scripting backend!\n"); 

   mantaDomain = mono_jit_init("MantaMono");

   mono_printf("Registering C->C# bindings...\n");
   RegisterCBindings();

   LoadAssemblies();

   mono_printf("Checking integrity of runtime!\n");
   TestRuntime();
}

void MonoScriptingBackend::LoadAssemblies() {
   // Folder should be at ./bin
   for (auto file : directory_iterator("./bin")) {
      printf("[Mono]: Attempting to open Mono Assembly at %s\n", file.path().c_str());
      
      MonoAssembly* assembly = mono_domain_assembly_open(mantaDomain, file.path().c_str());
      if (!assembly) {
	 printf("[Mono]: Failed to open...\n");
	 continue;
      }

      std::string name = file.path().stem().c_str();
      loadedAssemblies.emplace(name, assembly);
      printf("[Mono]: Opened and registered as %s!\n", name.c_str());
   }
}

void MonoScriptingBackend::TestRuntime() {
   auto iter = loadedAssemblies.find("Manta.Internal");

   if (iter != loadedAssemblies.end()) {
      MonoAssembly* assembly = iter->second;   
   
      MonoImage* mantaImage = mono_assembly_get_image(assembly);
      MonoClass* klass = mono_class_from_name(mantaImage, "MantaInternal", "MantaTests");

      if (!klass) {
	 mono_printf("Failed to find MantaInternal::MantaTests in compiled Manta dll!\nSomething either went wrong with the path or this build is broken!\n");
	 return;
      }

      MonoMethodDesc* testMethodDesc = mono_method_desc_new(":__test_method()", false);
      MonoMethod* testMethod = mono_method_desc_search_in_class(testMethodDesc, klass);
      mono_method_desc_free(testMethodDesc);

      if (!testMethod) {
	 mono_printf("Failed to find MantaTests::__test_method()\n");
	 return;
      }

      MonoObject* exception;
      mono_runtime_invoke(testMethod, nullptr, {}, &exception);
   }
}

// C -> C# bindings
static void mantamono_printf(MonoString* string) {
   printf("%s", mono_string_to_utf8(string));
}

void manta_register_mono_method(MonoScriptingBackend* backend, const char* methodName, void* cfunc) {
   std::string fullName = "MantaInternal.CBindings::__c__";
   fullName += methodName;
  
   if (backend) {
      if (backend->debug) {
	 printf("[Mono]: [Debug] Registered C->C# for %s\n", fullName.c_str());
      }
   }

   mono_add_internal_call(fullName.c_str(), cfunc);
}

void MonoScriptingBackend::RegisterCBindings() {
   manta_register_mono_method(this, "printf", (void*)mantamono_printf);
}
