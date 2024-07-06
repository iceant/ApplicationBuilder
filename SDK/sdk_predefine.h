#ifndef INCLUDED_SDK_PREDEFINE_H
#define INCLUDED_SDK_PREDEFINE_H

#ifndef SDK_CPU_BITS
    #define SDK_CPU_BITS 64
#endif

#ifndef SDK_USE_TLSF
    #define SDK_USE_TLSF 1
#endif

#if (SDK_USE_TLSF==1)
    #ifndef SDK_TLSF_POOL_SIZE
        #define SDK_TLSF_POOL_SIZE (1024*1024*10)
    #endif
#endif

#ifndef SDK_OBJECT_NAME_SIZE
    #define SDK_OBJECT_NAME_SIZE 32
#endif

#ifndef SDK_USE_POSIX
    #define SDK_USE_POSIX
#endif

#ifdef WIN32
    #ifndef SDK_USE_POSIX
        #define OS_WIN32 1
    #else
        #define OS_WIN32 0
    #endif
#endif

#endif /* INCLUDED_SDK_PREDEFINE_H */
