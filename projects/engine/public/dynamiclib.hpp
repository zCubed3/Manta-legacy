#ifndef MANTA_DYNAMICLIB_H
#define MANTA_DYNAMICLIB_H

// Wraps around OS specific dynamic libraries to provide a generic interface
class DynamicLib {
   public:
      ~DynamicLib();

      void* GetFunction(const char* signature);

#if defined(__linux__) || defined(__FreeBSD__)
      DynamicLib(void* handle);
#endif

#ifdef _WIN32
      DynamicLib(HMODULE handle);
#endif

   private:
#if defined(__linux__) || defined(__FreeBSD__)
      void* handle;
#endif

#ifdef _WIN32
      HINSTANCE module;
#endif
};

DynamicLib* LoadDynamicLib(const char* path);

#endif
