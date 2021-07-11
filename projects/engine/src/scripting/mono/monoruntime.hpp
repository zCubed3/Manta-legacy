#ifndef MANTA_MONO_BACKEND_H
#define MANTA_MONO_BACKEND_H

#include <string>
#include <unordered_map>

// These are only here to prevent header hell with mono
typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;

class MonoScriptingBackend {
   public:
      void Initialize();
      void LoadAssemblies();
      void RegisterCBindings();
      void TestRuntime();

      MonoDomain* mantaDomain;
      std::unordered_map<std::string, MonoAssembly*> loadedAssemblies;

      bool debug = false;
};

#endif
