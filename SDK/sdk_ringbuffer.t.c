#include <sdk_ringbuffer.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct object_s{
    int a;
    int b;
}object_t;

#define OBJECT_SIZE     sizeof(object_t)
#define OBJECT_COUNT    10
#define BLOCK_SIZE (OBJECT_COUNT * OBJECT_SIZE)

static uint8_t blocks[BLOCK_SIZE];


int main(int argc, char** argv)
{
    sdk_ringbuffer_t rbuf;
    sdk_ringbuffer_init(&rbuf, blocks, BLOCK_SIZE, OBJECT_SIZE);

    printf("block_size:%d, object_size:%d, buffer_count:%d\n",BLOCK_SIZE, rbuf.object_size, SDK_RINGBUFFER_CAPACITY(&rbuf));

//    for(int i=0; i<=SDK_RINGBUFFER_CAPACITY(&rbuf); i++){
    for(int i=0; i<=100; i++){
        object_t* slot = sdk_ringbuffer_put_slot(&rbuf);
        if(slot){
            slot->a = i;
            slot->b = i+10;
            printf("write [%d].a=%d\n", i, slot->a);
        }
    }

    printf("size(used): %d\n", sdk_ringbuffer_size(&rbuf));

    for(int i=0; i<sdk_ringbuffer_size(&rbuf); i++){
        object_t p;
        int err = sdk_ringbuffer_peek(&rbuf, i, &p);
        if(err!=0){
            printf("ERR: %d\n", err);
        }else{
            printf("[%d].a=%d\n", i, p.a);
        }
    }

    return 0;
}