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
    int err;
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
        }else{
            break;
        }
    }
    printf("Try to put object_1 into rb when rb is full!\n");
    object_t object_1={.a=11, .b=21};
    err = sdk_ringbuffer_put(&rbuf, &object_1);
    if(err!=SDK_RINGBUFFER_OK){
        printf("Put object_1 into ringbuffer failed! code=%d\n", err);
    }
    
    printf("read_idx: %d, write_idx: %d\n", rbuf.read_idx, rbuf.write_idx);
    printf("size(used): %d\n", sdk_ringbuffer_size(&rbuf));
    printf("capacity: %d\n", SDK_RINGBUFFER_CAPACITY(&rbuf));
    printf("available: %d\n", SDK_RINGBUFFER_AVAILABLE(&rbuf));

    printf("move read_idx = 5\n");
    sdk_ringbuffer_advance_read_idx(&rbuf, 5);
    
    printf("Try to peek item in rbuf...\n");
    for(int i=0; i<sdk_ringbuffer_size(&rbuf); i++){
        object_t p;
        int err = sdk_ringbuffer_peek(&rbuf, i, &p);
        if(err!=0){
            printf("ERR: %d\n", err);
        }else{
            printf("[%d].a=%d\n", i, p.a);
        }
    }
    
    printf("Pop item in rbuf...\n");
    while(sdk_ringbuffer_size(&rbuf)){
        sdk_ringbuffer_pop(&rbuf, 0);
    }
    printf("read_idx: %d, write_idx: %d\n", rbuf.read_idx, rbuf.write_idx);
    printf("size(used): %d\n", sdk_ringbuffer_size(&rbuf));
    printf("capacity: %d\n", SDK_RINGBUFFER_CAPACITY(&rbuf));
    printf("available: %d\n", SDK_RINGBUFFER_AVAILABLE(&rbuf));
    
    printf("Try to put object_1 into rbuf...\n");
    err = sdk_ringbuffer_put(&rbuf, &object_1);
    if(err!=SDK_RINGBUFFER_OK){
        printf("Put object_1 into ringbuffer failed! code=%d\n", err);
    }
    
    printf("read_idx: %d, write_idx: %d\n", rbuf.read_idx, rbuf.write_idx);
    printf("size(used): %d\n", sdk_ringbuffer_size(&rbuf));
    printf("capacity: %d\n", SDK_RINGBUFFER_CAPACITY(&rbuf));
    printf("available: %d\n", SDK_RINGBUFFER_AVAILABLE(&rbuf));
    return 0;
}