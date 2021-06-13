#include <dynamiclib.hpp>

#include <string.h>
#include <stdio.h>

#ifdef __linux__ 
#include <dlfcn.h>
#endif

//
// I am aware that this is mostly preprocessor spaghetti but I can assure you it's worth it in the end...
//

DynamicLib* LoadDynamicLib(const char* path) {

   if (path == nullptr)
      return nullptr;

#ifdef __linux__
   int pathLen = strlen(path);

   char* soPath = new char[pathLen + 3]; // Path + .so
   sprintf(soPath, "%s.so", path);

   printf("Attempting to open library at %s\n", soPath);
   void* handle = dlopen(soPath, RTLD_LAZY | RTLD_GLOBAL);

   delete[] soPath;

   if (handle == nullptr) {
      printf("Failed to find library @ %s\nReason: %s\n", path, dlerror());
      return nullptr;
   }

   return new DynamicLib(handle);
#endif

   return nullptr;
}

//
// DynamicLib implementations
//

#ifdef __linux__
DynamicLib::DynamicLib(void* handle) {
   this->handle = handle;
}
#endif

DynamicLib::~DynamicLib() {
#ifdef __linux__
   if (handle != nullptr)
      dlclose(handle);
#endif

#ifdef __WIN32
   if (module != nullptr)
      FreeLibrary(module)
#endif
}

void* DynamicLib::GetFunction(const char *signature) {
#ifdef __linux__
   if (handle == nullptr) {
      printf("DynamicLib handle is invalid, returning invalid pointer\n");
      return nullptr;
   }

   return dlsym(handle, signature);
#endif
}
