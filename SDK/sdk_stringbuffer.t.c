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

    sdk_size_t idx = sdk_stringbuffer_find(&sbuf, "1234", 4);
    printf("find(1234)=%d\n", idx);

    int err = sdk_stringbuffer_advance_read_idx(&sbuf, idx);
    printf("sdk_stringbuffer_advance_read_idx: code=%d, read_idx=%d\n", err, sbuf.read_idx);

    unsigned long value = sdk_ringbuffer_strtoul(&sbuf, &idx, 10);
    printf("Long: %lu, idx=%d, read_idx=%d", value, idx, sbuf.read_idx);
    return 0;
}