#include <sdk_scopeguard.h>
#include <stdio.h>
#include <stdlib.h>

static void free_wrapper(void* data){
    printf("free(%p)\n", data);
    if(data){
        free(data);
    }
}

int main(int argc, char** argv)
{
    void* m100 = malloc(100);
    printf("alloc(m100) %p\n", m100);
    SDK_SCOPE_BEGIN(1)
        SDK_SCOPE_ONEXIT(1, free_wrapper, m100);
        SDK_SCOPE_BEGIN(1_1)
        {
            void* m200 = malloc(200);
            printf("alloc(m200) %p\n", m200);
            SDK_SCOPE_ONEXIT(1_1, free_wrapper, m200);
        }
        SDK_SCOPE_END(1_1)
    SDK_SCOPE_END(1)

    return 0;
}