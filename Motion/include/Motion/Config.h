#ifndef MOTION_CONFIG_H
#define MOTION_CONFIG_H

#if defined(_WIN32) || defined(__WIN32__)
    #define MOTION_SYSTEM_WINDOWS
#elif defined(linux) || defined(__linux)
    #define MOTION_SYSTEM_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    #define MOTION_SYSTEM_MACOS
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    #define MOTION_SYSTEM_FREEBSD
#else
    #error This operating system is not supported
#endif

#if defined(MOTION_SYSTEM_WINDOWS)
    #define MOTION_C_API_EXPORT extern "C" __declspec(dllexport)
    #define MOTION_C_API_IMPORT extern __declspec(dllimport)
    #define MOTION_CXX_API_EXPORT __declspec(dllexport)
    #define MOTION_CXX_API_IMPORT __declspec(dllimport)
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif
#else
    #if __GNUC__ >= 4
        #define MOTION_C_API_EXPORT extern "C" __attribute__ ((__visibility__ ("default")))
        #define MOTION_C_API_IMPORT extern __attribute__ ((__visibility__ ("default")))
        #define MOTION_CXX_API_EXPORT __attribute__ ((__visibility__ ("default")))
        #define MOTION_CXX_API_IMPORT __attribute__ ((__visibility__ ("default")))
    #else
        #define MOTION_C_API_EXPORT extern "C"
        #define MOTION_C_API_IMPORT extern
        #define MOTION_CXX_API_EXPORT
        #define MOTION_CXX_API_IMPORT
    #endif
#endif

#endif
