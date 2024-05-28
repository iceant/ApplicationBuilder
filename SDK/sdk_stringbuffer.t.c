#include <sdk_stringbuffer.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE 1024
static uint8_t sbuf_blocks[BLOCK_SIZE];

int main(int argc, char** argv){
    sdk_stringbuffer_t sbuf;
    sdk_stringbuffer_init(&sbuf, sbuf_blocks, BLOCK_SIZE);

    printf("stringbuffer.init, size=%d\n", sdk_stringbuffer_size(&sbuf));

    for(int i=0; i<256; i++){
        sdk_stringbuffer_put(&sbuf, i);
    }

    printf("after put 255 char into stringbuffer, size=%d\n", sdk_stringbuffer_size(&sbuf));

    for(int i=0; i<sdk_stringbuffer_size(&sbuf); i++){
        printf("[%d] %c \n", i, sdk_stringbuffer_get(&sbuf, i));
    }

    sdk_size_t idx = sdk_stringbuffer_find(&sbuf, 0, "1234", 4);
    printf("find(1234)=%d\n", idx);

    if(idx!=SDK_STRINGBUFFER_INVALID_INDEX){
        int err = sdk_stringbuffer_advance_read_idx(&sbuf, idx);
        printf("sdk_stringbuffer_advance_read_idx: code=%d, read_idx=%d\n", err, sbuf.read_idx);
    }

    unsigned long value = sdk_ringbuffer_strtoul(&sbuf, &idx, 10);
    printf("Long: %lu, idx=%d, read_idx=%d\n", value, idx, sbuf.read_idx);

    sdk_ringbuffer_put_u16(&sbuf, 0xaabb, kSDK_ByteOrder_BigEndian);
    sdk_ringbuffer_put_u16(&sbuf, 0xaabb, kSDK_ByteOrder_LittleEndian);

    printf("write_idx:%d, read_idx:%d\n", sbuf.write_idx, sbuf.read_idx);

    sdk_size_t size = sdk_stringbuffer_size(&sbuf);
    sdk_stringbuffer_advance_read_idx(&sbuf, size-4);
//    sbuf.read_idx = sbuf.write_idx-4;
//    for(int i=0; i<sdk_stringbuffer_size(&sbuf); i++){
//        printf("[%d] %02x\n", i, sdk_stringbuffer_get(&sbuf, i)& 0xff);
//    }


    printf("u16:%x\n", sdk_ringbuffer_read_u16(&sbuf, 0, kSDK_ByteOrder_BigEndian));
    printf("u16:%x\n", sdk_ringbuffer_read_u16(&sbuf, 2, kSDK_ByteOrder_LittleEndian));

    return 0;
}

