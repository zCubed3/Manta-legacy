#ifndef MANTA_MACROS_H
#define MANTA_MACROS_H

#define MANTA_FLAG(SHIFT) 1 << SHIFT

#define MANTA_DECLARE_FPTR(RETURN, TYPE, ...) typedef RETURN(*TYPE)(__VA_ARGS__)

#define RGB_TO_LINEAR(V) (V / 255.0f)

//
// Platform specific defines
//

#ifdef __linux__
#define MANTA_EXPORT extern "C"
#endif

#ifdef _WIN32
#define MANTA_EXPORT extern "C" __declspec(dllexport)
#endif

#endif
