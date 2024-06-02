#include <sdk_thread.h>
#include <stdio.h>
#include <stdlib.h>


static void thread_work(void * ){
    sdk_size_t nCount = 0;
    while(1){
        if(nCount==10){
            break;
        }
        printf("Thread: %p , nCount=%d\n", sdk_thread_self(), nCount++);
        sdk_thread_delayms(1000);
    }
}

int main(int argc, char** argv)
{
    sdk_thread_t thread;
    sdk_thread_init(&thread, thread_work, 0, 0, 0, kSDK_ThreadPriority_Normal, 0);
    sdk_thread_startup(&thread);
    sdk_thread_join(&thread);
    return 0;
}