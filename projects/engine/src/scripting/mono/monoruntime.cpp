#include "monoruntime.hpp"
#include <mono/metadata/appdomain.h>
#include <mono/metadata/debug-helpers.h>

#include <manta_macros.hpp>

void MonoScriptingBackend::Initialize() {
   printf("Initializing Mono scripting backend!\n"); 

   monoDomain = mono_jit_init("MantaMonoDomain");

   mantaAssembly = mono_domain_assembly_open(monoDomain, "MantaMono.dll");

   if (!mantaAssembly) {
      printf("Failed to open MantaMono.dll, aborting engine!\n");
      exit(1);
   }

   RegisterCBindings();

   MonoImage* mantaImage = mono_assembly_get_image(mantaAssembly);
   MonoClass* testSuiteClass = mono_class_from_name(mantaImage, "MantaMonoInternal", "MantaTests");

   if (!testSuiteClass) {
      printf("Failed to find MantaTests in compiled mono dll! This may be a borked compilation!\n");
      exit(1);
   }

   MonoMethodDesc* heartbeatDesc = mono_method_desc_new(":__manta_heartbeat()", false);
   MonoMethod* heartbeatMethod = mono_method_desc_search_in_class(heartbeatDesc, testSuiteClass);
   mono_method_desc_free(heartbeatDesc);

   MonoObject* exception;
   mono_runtime_invoke(heartbeatMethod, nullptr, {}, &exception);
}

// C -> C# bindings
static void mantamono_printf(MonoString* string) {
   printf("%s", mono_string_to_utf8(string));
}

void MonoScriptingBackend::RegisterCBindings() {
   mono_add_internal_call("MantaInternal.CMethodBindings::__c__printf", (void*)mantamono_printf);
}
