#ifndef INCLUDED_SDK_THREAD_H
#define INCLUDED_SDK_THREAD_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#if (OS_WIN32)
    #include <windows.h>
#else
    #include <pthread.h>
    #include <time.h>
#endif

////////////////////////////////////////////////////////////////////////////////
////

typedef enum sdk_thread_priority_enum{
    kSDK_ThreadPriority_Idle = -15,
    kSDK_ThreadPriority_Lowest = -2,
    kSDK_ThreadPriority_BelowNormal = -1,
    kSDK_ThreadPriority_Normal = 0,
    kSDK_ThreadPriority_AboveNormal = 1,
    kSDK_ThreadPriority_Highest = 2,
    kSDK_ThreadPriority_Critical = 15,
}sdk_thread_priority_t;

typedef void (*sdk_thread_function_t)(void* parameter);

typedef struct sdk_thread_s sdk_thread_t;


#define SDK_THREAD_COMMON_FIELDS \
    sdk_thread_function_t thread_entry; \
    void* parameter;             \
    void* stack_address;         \
    sdk_size_t stack_size;       \
    sdk_thread_priority_t init_priority;    \
    sdk_uint_t time_slices;

#if (OS_WIN32)
struct sdk_thread_s{
    HANDLE handle;
    SDK_THREAD_COMMON_FIELDS
    DWORD thread_id;
};
#else
struct sdk_thread_s{
        pthread_t handle;
        SDK_THREAD_COMMON_FIELDS
        void* exit_res;
    };
#endif

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_thread_init(sdk_thread_t* thread, sdk_thread_function_t thread_entry, void* parameter
    , void* stack_addr, sdk_size_t stack_size
    , sdk_thread_priority_t priority, sdk_uint_t time_slices);

sdk_err_t sdk_thread_startup(sdk_thread_t* thread);

sdk_err_t sdk_thread_join(sdk_thread_t * thread);

void* sdk_thread_self(void);

sdk_err_t sdk_thread_delayms(unsigned long time_ms);

#endif /* INCLUDED_SDK_THREAD_H */
