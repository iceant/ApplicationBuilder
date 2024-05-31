#include <sdk_crc8.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    unsigned char test[] = { 0xC0, 0x0F, 0xD6 };

    printf("crc=0x%02X Fast\n", sdk_crc8(0x00, test,  sizeof(test)));

    return 0;
}
