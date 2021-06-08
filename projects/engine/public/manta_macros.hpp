#ifndef MANTA_MACROS_H
#define MANTA_MACROS_H

#define MANTA_FLAG(SHIFT) 1 << SHIFT

#define MANTA_DECLARE_FPTR(RETURN, TYPE, ARGS) typedef RETURN(*TYPE)(ARGS)

// Used for dynamic linking
#if defined(__linux__) || defined(__FreeBSD)
#define MANTA_EXPORT extern "C"
#endif

#ifdef __WIN32
#define MANTA_EXPORT extern "C" __declspec(dllexport)
#endif

#endif
