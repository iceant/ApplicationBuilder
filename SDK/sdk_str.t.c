#include <sdk_str.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    char* str = "Free software is a matter of liberty, not price.\n"
                "To understand the concept, you should think of 'free' \n"
                "as in 'free speech', not as in 'free beer'";

    char* replaced = sdk_str_replace(str, "ree", "AV");

    printf("str address: %p\n", str);
    printf("replaced address: %p\n", replaced);
    printf("replaced(%d): \n%s\n", sdk_str_len(replaced, 1, 0), replaced);

    sdk_str_free(replaced);

    return 0;
}