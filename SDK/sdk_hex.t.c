#include <sdk_hex.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define HEX_STRING_SIZE (BUFFER_SIZE*2+1)

static uint8_t buffer[BUFFER_SIZE];
static char hex_string[HEX_STRING_SIZE];

int main(int argc, char** argv){

    int i;
    for(i=0; i<sizeof(buffer); i++){
        buffer[i] = i;
    }

    sdk_hex_dump("buffer", buffer, i);

    SDK_HEX_ENCODE_BE(hex_string, sizeof(hex_string), buffer, i);
    SDK_HEX_ENCODE_LE(hex_string, sizeof(hex_string), buffer, i);
    printf("hex: %s\n", hex_string);

    return 0;
}