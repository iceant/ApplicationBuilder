#include <sdk_md5.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    uint8_t md5[16];
    sdk_md5_string("hello from md5", md5);
    /* 698BAD8DC2ACF064F1038A5BC204B048 */
    for(int i=0; i<sizeof(md5); i++){
        printf("%02X", md5[i]);
    }
    printf("\n");

    return 0;
}