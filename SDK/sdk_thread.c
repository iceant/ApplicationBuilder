#include <sdk_thread.h>

////////////////////////////////////////////////////////////////////////////////
////


sdk_err_t sdk_thread_init(sdk_thread_t* thread, sdk_thread_function_t thread_entry, void* parameter
        , void* stack_addr, sdk_size_t stack_size
        , sdk_thread_priority_t priority, sdk_uint_t time_slices){
    
    thread->thread_entry = thread_entry;
    thread->parameter = parameter;
    thread->stack_size = stack_size;
    thread->stack_address = stack_addr;
    thread->init_priority = priority;
    thread->time_slices = time_slices;
    
    return SDK_EOK;
}

sdk_err_t sdk_thread_startup(sdk_thread_t* thread)
{
#if OS_WIN32
//    DWORD dwCreationFlags = 0 /* Run Immediately */;
//    DWORD dwCreationFlags = CREATE_SUSPENDED /* Create in suspend state */;
//    DWORD dwCreationFlags = STACK_SIZE_PARAM_IS_A_RESERVATION /* Create in suspend state */;
    thread->handle = CreateThread(NULL, thread->stack_size, (LPTHREAD_START_ROUTINE) thread->thread_entry, thread->parameter
                                  , CREATE_SUSPENDED|STACK_SIZE_PARAM_IS_A_RESERVATION
                                  , &thread->thread_id);
    SetThreadPriority(thread->handle, (int)thread->init_priority);
    ResumeThread(thread->handle);
#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if(thread->stack_address!=0 && thread->stack_size!=0){
        pthread_attr_setstackaddr(&attr, thread->stack_address);
        pthread_attr_setstacksize(&attr, thread->stack_size);
    }
    int err = pthread_create(&thread->handle, &attr, (void *(*)(void *)) thread->thread_entry, thread->parameter);
    pthread_attr_destroy(&attr);
    
    if(err!=0){
        return SDK_ERROR;
    }

#endif
    
    return SDK_EOK;
}

sdk_err_t sdk_thread_join(sdk_thread_t * thread)
{
    #if OS_WIN32
    WaitForSingleObject(thread->handle, INFINITE);
    #else
    pthread_join(thread->handle,&thread->exit_res);
    #endif
    
    return SDK_EOK;
}

void* sdk_thread_self(void)
{
    #if OS_WIN32
    GetCurrentThreadId();
    #else
    return (void *) pthread_self();
    #endif
}

sdk_err_t sdk_thread_delayms(unsigned long milliseconds)
{
#if OS_WIN32
    Sleep(milliseconds);
#else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}
