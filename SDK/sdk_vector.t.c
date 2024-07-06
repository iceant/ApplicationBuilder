#include <sdk_vector.h>
#include <sdk_memory.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    sdk_err_t err = 0;
    
    sdk_vector_t v;
    err = sdk_vector_init(&v, 0);
    if(err!=SDK_VECTOR_OK){
        printf("Vector Init Failed! Code=%d\n", err);
    }
    
    for(int i=0; i<10; i++){
        err = sdk_vector_resize_add(&v, (void*)(i+1));
        if(err!=SDK_VECTOR_OK){
            printf("add failed! code=%d\n", err);
            sdk_vector_destroy(&v);
            return -1;
        }
    }
    
    printf("vector.size=%d\n", v.size);
    
    for(int i=0; i< v.size; i++){
        printf("[%d] %d\n", i, (int)sdk_vector_get(&v, i));
    }

    while(v.size){
        sdk_vector_resize_remove(&v, 0);
        printf("vector.size=%d, capacity=%d\n", v.size, v.capacity);
    }
    
    sdk_vector_destroy(&v);
    
    
    return 0;
}
