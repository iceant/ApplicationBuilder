#ifndef INCLUDED_SDK_COUNTDOWNLATCH_H
#define INCLUDED_SDK_COUNTDOWNLATCH_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#if OS_WIN32
    #ifndef INCLUDED_WINDOWS_H
    #define INCLUDED_WINDOWS_H
    #include <windows.h>
    #endif /*INCLUDED_WINDOWS_H*/
#else
    #ifndef INCLUDED_PTHREAD_H
    #define INCLUDED_PTHREAD_H
    #include <pthread.h>
    #endif /*INCLUDED_PTHREAD_H*/
#endif

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_countdownlatch_s{
    sdk_size_t count;
    #if OS_WIN32
    CRITICAL_SECTION   mutex;
    CONDITION_VARIABLE condv;
    #else
    pthread_mutex_t mutex;
    pthread_cond_t  condv;
    #endif
}sdk_countdownlatch_t;


////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_countdownlatch_init(sdk_countdownlatch_t * latch, sdk_size_t count);

void sdk_countdownlatch_destroy(sdk_countdownlatch_t * latch);

sdk_err_t sdk_countdownlatch_down(sdk_countdownlatch_t * latch);

sdk_err_t sdk_countdownlatch_await(sdk_countdownlatch_t * latch);



#endif /* INCLUDED_SDK_COUNTDOWNLATCH_H */
