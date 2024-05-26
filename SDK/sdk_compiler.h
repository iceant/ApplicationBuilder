#ifndef INCLUDED_SDK_COMPILER_H
#define INCLUDED_SDK_COMPILER_H

////////////////////////////////////////////////////////////////////////////////
////

#if defined(_MSC_VER)
    #pragma section(".CRT$XCU",read)
    #define SDK_C_INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
    #ifdef _WIN64
        #define SDK_C_CONSTRUCTOR(f) SDK_C_INITIALIZER2_(f,"")
    #else
        #define SDK_C_CONSTRUCTOR(f) SDK_C_INITIALIZER2_(f,"_")
    #endif
#else
    #define SDK_C_CONSTRUCTOR(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
        
    #define SDK_C_DESTRUCTOR(f) \
        static void f(void) __attribute__((destructor)); \
        static void f(void)
#endif


#if defined(__GNUC__)
    #ifndef SDK_C_ASM
        #define SDK_C_ASM( S ) __asm
    #endif

    #ifndef SDK_C_SECTION
        #define SDK_C_SECTION( S ) __attribute__((section ( #S )))
    #endif
    
    #ifndef SDK_C_STATICFORCEINLINE
        #define SDK_C_STATICFORCEINLINE static __attribute__((always_inline))
    #endif
    
    #ifndef SDK_C_WEAK
        #define SDK_C_WEAK __attribute__((weak))
    #endif
#endif

#if defined(_MSC_VER)
    #ifndef SDK_C_ASM
            #define SDK_C_ASM( S ) __asm
    #endif
    
    #ifndef SDK_C_SECTION
        #define SDK_C_SECTION( S ) __declspec(allocate(#S))
    #endif
    
    #ifndef SDK_C_STATICFORCEINLINE
        #define SDK_C_STATICFORCEINLINE static __forceinline
    #endif
#endif

#endif /* INCLUDED_SDK_COMPILER_H */
