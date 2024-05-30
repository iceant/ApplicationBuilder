#include "sdk_hashtable.h"
#include <stdlib.h>
#include <stdio.h>

static void node_print(sdk_hashtable_node_t* node, void* ud){
    printf("[%08x] %s=%s\n", node->hash, (const char*)node->key, (const char*)node->value);
}

int main(int argc, char** argv){

    sdk_hashtable_t htable;
    sdk_hashtable_init(&htable, 3, sdk_hashtable_string_key_ops, sdk_hashtable_string_val_ops);

    char k_buf[256];
    char v_buf[256];

    for(int i=0; i<10; i++){
        snprintf(k_buf, sizeof(k_buf), "Key-%d", i);
        snprintf(v_buf, sizeof(v_buf), "Val-%d", i);
        sdk_hashtable_put(&htable, k_buf, v_buf);
    }

    sdk_hashtable_foreach(&htable, node_print, 0);

    printf("num col: %d\n", sdk_hashtable_numcol(&htable));
    printf("capacity: %d, size:%d\n", htable.capacity, htable.size);
    sdk_hashtable_destroy(&htable);

    return 0;
}