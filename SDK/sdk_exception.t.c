#include <sdk_exception.h>
#include <stdlib.h>
#include <stdio.h>

sdk_exception_t sdk_exception__DIVZERO={"Div Zero"};

sdk_exception_t Allocate_Failed={"Allocate Failed!"};

void* allocate(int size){
    if(size>1024){
        SDK_RAISE(Allocate_Failed);
        return 0;
    }
    return malloc(size);
}

int main(int argc, char** argv){
    char* buf;
    SDK_TRY
        buf = allocate(4096);
    SDK_EXCEPT(Allocate_Failed)
        fprintf(stderr, "Couldn't allocate the buffer\n");
        exit(EXIT_FAILURE);
    SDK_END_TRY

    if(buf){
        free(buf);
    }

    return 0;
}