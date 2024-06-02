#include <sdk_countdownlatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_thread.h>
#include "sdk_macros.h"

static sdk_countdownlatch_t worker_latch;

static void thread_work(void* p){
    int nCount = 0;
    sdk_countdownlatch_t * latch = p;
    printf("Thread %p waiting for start...\n", sdk_thread_self());
    sdk_countdownlatch_await(latch); /* wait for main thread */
    while(1){
        if(nCount==10){
            break;
        }
        printf("Thread %p, Count=%d\n", sdk_thread_self(), nCount++);
        sdk_thread_delayms(1000);
    }
    sdk_countdownlatch_down(&worker_latch);
}

int main(int argc, char** argv)
{
    sdk_thread_t threads[4];
    
    sdk_countdownlatch_t start_latch;

    
    sdk_countdownlatch_init(&start_latch, 1);
    sdk_countdownlatch_init(&worker_latch, SDK_ARRAY_SIZE(threads));
    
    printf("Creating Threads...\n");
    for(int i=0; i<SDK_ARRAY_SIZE(threads); i++){
        sdk_thread_init(&threads[i], thread_work, &start_latch, 0, 0, kSDK_ThreadPriority_Normal, 0);
        sdk_thread_startup(&threads[i]);
    }
    
    sdk_thread_delayms(5000);
    printf("Start Threads...\n");
    sdk_countdownlatch_down(&start_latch);
    
    printf("Wait for worker latch done!\n");
    sdk_countdownlatch_await(&worker_latch);
    
    sdk_countdownlatch_destroy(&start_latch);
    sdk_countdownlatch_destroy(&worker_latch);
    
    printf("All done!\n");
    return 0;
}