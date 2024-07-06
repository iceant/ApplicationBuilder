#include <sdk_memory.h>

#if (SDK_USE_TLSF==1)

#include <tlsf.h>
#include <stdio.h>

#if(__WIN32)
    #include <errhandlingapi.h>
#include <signal.h>

#else
    #include <signal.h>
#endif

/* ---------------------------------------------------------------------------------------------------- */

static void* sdk_memory__tlsf_pool=0;

void sdk_memory__exit_hook(int signal)
{
    if(sdk_memory__tlsf_pool){
        printf("sdk_memory__exit_hook: %d\n", signal);
        std_free(sdk_memory__tlsf_pool);
        sdk_memory__tlsf_pool = 0;
    }
    if(signal==SIGFPE){
        exit(-SIGFPE);
    }
}

SDK_C_CONSTRUCTOR(sdk_memory__ctor)
{
    #if(__WIN32)
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)&sdk_memory__exit_hook);
    #else
    signal(SIGFPE, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGSEGV, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGABRT, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGILL, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGINT, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGTERM, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGSTOP, (void(*)(int)) sdk_memory__exit_hook);
    signal(SIGKILL, (void(*)(int)) sdk_memory__exit_hook);
    atexit((void (*)(void)) sdk_memory__exit_hook);
    #endif
    
    sdk_memory__tlsf_pool = std_malloc(SDK_TLSF_POOL_SIZE);
    if(!sdk_memory__tlsf_pool) return;
    
    sdk_size_t size = sdk_memory_init(sdk_memory__tlsf_pool, SDK_TLSF_POOL_SIZE);
    printf("sdk_memory_init: %u/%u(%u)\n", size, SDK_TLSF_POOL_SIZE, (SDK_TLSF_POOL_SIZE-size));
    
}

////////////////////////////////////////////////////////////////////////////////
////

sdk_size_t sdk_memory_init(void* memory_pool, sdk_size_t size)
{
    return init_memory_pool(size, memory_pool);
}

void sdk_memory_destroy(void* memory_pool)
{
    destroy_memory_pool(memory_pool);
}

void* sdk_memory_malloc(sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return tlsf_realloc(memory, nBytes);
}

void  sdk_memory_free(void* memory, const char* file, sdk_size_t line)
{
    if(memory){
        tlsf_free(memory);
    }
}

void* sdk_memory_std_malloc(sdk_size_t nBytes)
{
    return malloc(nBytes);
}

void* sdk_memory_std_calloc(sdk_size_t nCount, sdk_size_t nBytes)
{
    return calloc(nCount, nBytes);
}

void* sdk_memory_std_realloc(void* mem, sdk_size_t nBytes)
{
    return realloc(mem, nBytes);
}

void  sdk_memory_std_free(void* mem)
{
    free(mem);
}

#else

#include <stdlib.h>

void sdk_memory_init(void* memory_pool, sdk_size_t size)
{
}

void sdk_memory_destroy(void* memory_pool)
{
}

void* sdk_memory_malloc(sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes, const char* file, sdk_size_t line)
{
    return realloc(memory, nBytes);
}

void  sdk_memory_free(void* memory, const char* file, sdk_size_t line)
{
    if(memory){
        free(memory);
    }
}

void* sdk_memory_c_malloc(sdk_size_t nBytes)
{
    return malloc(nBytes);
}

void* sdk_memory_c_calloc(sdk_size_t nCount, sdk_size_t nBytes){
    return calloc(nCount, nBytes);
}

void* sdk_memory_c_realloc(void* mem, sdk_size_t nBytes)
{
    return realloc(mem, nBytes);
}

void  sdk_memory_c_free(void* mem){
    free(mem);
}
#endif
