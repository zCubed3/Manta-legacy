#include <dynamiclib.hpp>

#include <stdio.h>

#if defined(__linux__) || defined(__FreeBSD__)
#include <dlfcn.h>
#endif

// I am aware that this is mostly preprocessor spaghetti but I can assure you it's worth it in the end...
DynamicLib* LoadDynamicLib(const char* path) {

   if (path == nullptr)
      return nullptr;

#if defined(__linux__) || defined(__FreeBSD__)
   void* handle = dlopen(path, RTLD_LAZY);

   if (handle == nullptr) {
      printf("Failed to find library @ %s\n", path);
      return nullptr;
   }

   return new DynamicLib(handle);
#endif

   return nullptr;
}

//
// DynamicLib class definitions
//

#if defined(__linux__) || defined(__FreeBSD__)
DynamicLib::DynamicLib(void* handle) {
   this->handle = handle;
}
#endif

DynamicLib::~DynamicLib() {
#if defined(__linux__) || defined(__FreeBSD__)
   if (handle != nullptr)
      dlclose(handle);
#endif

#ifdef __WIN32
   if (module != nullptr)
      FreeLibrary(module)
#endif
}

void* DynamicLib::GetFunction(const char *signature) {
#if defined(__linux__) || defined(__FreeBSD__)
   if (handle == nullptr) {
      printf("DynamicLib handle is invalid, returning invalid pointer\n");
      return nullptr;
   }

   return dlsym(handle, signature);
#endif
}
