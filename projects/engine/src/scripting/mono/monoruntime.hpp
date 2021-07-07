#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class MonoScriptingBackend {
   public:
      void Initialize();
      void RegisterCBindings();

      MonoDomain* monoDomain;
      MonoAssembly* mantaAssembly; // This is required, if we can't load MantaMono.dll, then mono doesn't work
};
