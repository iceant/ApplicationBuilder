#ifndef INCLUDED_SDK_MEMORY_H
#define INCLUDED_SDK_MEMORY_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#ifndef INCLUDED_STDLIB_H
#define INCLUDED_STDLIB_H
#include <stdlib.h>
#endif /*INCLUDED_STDLIB_H*/


////////////////////////////////////////////////////////////////////////////////
////

sdk_size_t sdk_memory_init(void* memory_pool, sdk_size_t size);
void sdk_memory_destroy(void* memory_pool);

void* sdk_memory_malloc(sdk_size_t nBytes, const char* file, sdk_size_t line);
void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line);
void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line);
void  sdk_memory_free(void* memory, const char* file, sdk_size_t line);

#if (SDK_USE_TLSF==1)
void* sdk_memory_std_malloc(sdk_size_t nBytes);
void* sdk_memory_std_calloc(sdk_size_t nCount, sdk_size_t nBytes);
void* sdk_memory_std_realloc(void* mem, sdk_size_t nBytes);
void  sdk_memory_std_free(void* mem);
#endif
////////////////////////////////////////////////////////////////////////////////
////

#define SDK_ALLOC(SZ) sdk_memory_malloc((SZ), __FILE__, __LINE__)
#define SDK_CALLOC(N, SZ) sdk_memory_malloc((N), (SZ), __FILE__, __LINE__)
#define SDK_REALLOC(P, SZ) sdk_memory_realloc((P), (SZ), __FILE__, __LINE__)
#define SDK_RESIZE(P, SZ) ((P)=sdk_memory_realloc((P), (SZ), __FILE__, __LINE__))
#define SDK_FREE(P) (sdk_memory_free((P), __FILE__, __LINE__), (P)=0)

#define SDK_NEW(P) (P)=SDK_ALLOC(sizeof(*(P)))
#define SDK_NEW0(P) (P)=SDK_CALLOC(1, sizeof(*(P)))

#if (SDK_USE_TLSF==1)
    #define std_malloc      sdk_memory_std_malloc
    #define std_calloc      sdk_memory_std_calloc
    #define std_realloc     sdk_memory_std_realloc
    #define std_free        sdk_memory_std_free
#else
    #define std_malloc      malloc
    #define std_calloc      calloc
    #define std_realloc     realloc
    #define std_free        free
#endif

#endif /* INCLUDED_SDK_MEMORY_H */
