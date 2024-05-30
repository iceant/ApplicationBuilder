#include <wui_hash.h>
#include <sdk_memory.h>

////////////////////////////////////////////////////////////////////////////////
////


uint32_t sdk_uint_hash(const void *data, void *arg) {
    return (size_t)data;
}

void* sdk_uint_cp(const void *data, void *arg) {
    return (void*)data;
}

bool sdk_uint_eq(const void *data1, const void *data2, void *arg) {
    return (data1==data2)?true:false;
}

void sdk_uint_free(void *data, void *arg) {}


////////////////////////////////////////////////////////////////////////////////
////
uint32_t sdk_uintptr_hash(const void *data, void *arg) {
    return (uintptr_t)data;
}

void* sdk_uintptr_cp(const void *data, void *arg) {
    return data;
}

bool sdk_uintptr_eq(const void *data1, const void *data2, void *arg) {
    return (data1==data2)?true:false;
}

void sdk_uintptr_free(void *data, void *arg) {}



////////////////////////////////////////////////////////////////////////////////
////


sdk_hashtable_key_ops_t wui_hash_uint_key_ops={.hash=sdk_uint_hash, .cp=sdk_uint_cp, .free=sdk_uint_free, .eq=sdk_uint_eq, .arg=0};

sdk_hashtable_val_ops_t wui_hash_uintptr_val_ops={.cp=sdk_uintptr_cp, .free=sdk_uintptr_free, .eq=sdk_uintptr_eq, .arg=0};

