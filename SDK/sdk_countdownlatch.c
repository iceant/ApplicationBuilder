#include <sdk_countdownlatch.h>



sdk_err_t sdk_countdownlatch_init(sdk_countdownlatch_t * latch, sdk_size_t count)
{
    #if OS_WIN32
    InitializeCriticalSection(&latch->mutex);
    InitializeConditionVariable(&latch->condv);
    #else
    pthread_mutex_init(&latch->mutex, 0);
    pthread_cond_init(&latch->condv, 0);
    #endif
    
    latch->count = count;
    
    return SDK_EOK;
}

void sdk_countdownlatch_destroy(sdk_countdownlatch_t* latch){
    #if OS_WIN32
    DeleteCriticalSection(&latch->mutex);
    #else
    pthread_cond_destroy(&latch->condv);
    pthread_mutex_destroy(&latch->mutex);
    #endif
}


sdk_err_t sdk_countdownlatch_down(sdk_countdownlatch_t * latch)
{
    #if OS_WIN32
    EnterCriticalSection(&latch->mutex);
    latch->count--;
    if(latch->count==0){
        LeaveCriticalSection(&latch->mutex);
        WakeAllConditionVariable(&latch->condv);
        return SDK_EOK;
    }
    LeaveCriticalSection(&latch->mutex);
    #else
    pthread_mutex_lock(&latch->mutex);
    latch->count--;
    if(latch->count==0){
        pthread_mutex_unlock(&latch->mutex);
        pthread_cond_broadcast(&latch->condv);
        return SDK_EOK;
    }
    pthread_mutex_unlock(&latch->mutex);
    #endif
    
    return SDK_EOK;
}

sdk_err_t sdk_countdownlatch_await(sdk_countdownlatch_t * latch)
{
    #if OS_WIN32
    EnterCriticalSection(&latch->mutex);
    while(latch->count){
        SleepConditionVariableCS(&latch->condv, &latch->mutex, INFINITE);
    }
    LeaveCriticalSection(&latch->mutex);
    #else
    pthread_mutex_lock(&latch->mutex);
    while(latch->count){
        pthread_cond_wait(&latch->condv, &latch->mutex);
    }
    pthread_mutex_unlock(&latch->mutex);
    #endif
    return SDK_EOK;
}

