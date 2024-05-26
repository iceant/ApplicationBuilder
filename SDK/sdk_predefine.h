#ifndef INCLUDED_SDK_PREDEFINE_H
#define INCLUDED_SDK_PREDEFINE_H

#ifndef SDK_CPU_BITS
    #define SDK_CPU_BITS 32
#endif

#ifndef SDK_USE_TLSF
    #define SDK_USE_TLSF 1
#endif

#if (SDK_USE_TLSF==1)
    #ifndef SDK_TLSF_POOL_SIZE
        #define SDK_TLSF_POOL_SIZE (1024*1024)
    #endif
#endif



#endif /* INCLUDED_SDK_PREDEFINE_H */
