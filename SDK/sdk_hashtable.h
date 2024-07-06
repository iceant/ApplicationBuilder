#ifndef INCLUDED_SDK_HASHTABLE_H
#define INCLUDED_SDK_HASHTABLE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef struct sdk_hashtable_key_ops_s {
    uint32_t (*hash)(const void *data, void *arg);
    void* (*cp)(const void *data, void *arg);
    void (*free)(void *data, void *arg);
    bool (*eq)(const void *data1, const void *data2, void *arg);
    void *arg;
} sdk_hashtable_key_ops_t;

typedef struct sdk_hashtable_val_ops_s {
    void* (*cp)(const void *data, void *arg);
    void (*free)(void *data, void *arg);
    bool (*eq)(const void *data1, const void *data2, void *arg);
    void *arg;
} sdk_hashtable_val_ops_t;

typedef struct sdk_hashtable_node_s{
    uint32_t hash;
    void* key;
    void* value;
}sdk_hashtable_node_t;

typedef struct sdk_hashtable_s{
    sdk_size_t capacity;
    sdk_size_t size;
    sdk_vector_t ** buckets;
    sdk_hashtable_key_ops_t key_ops;
    sdk_hashtable_val_ops_t val_ops;
}sdk_hashtable_t;


////////////////////////////////////////////////////////////////////////////////
////
sdk_err_t sdk_hashtable_init(sdk_hashtable_t * htable, sdk_size_t capacity, sdk_hashtable_key_ops_t key_ops, sdk_hashtable_val_ops_t val_ops);

void sdk_hashtable_destroy(sdk_hashtable_t * htable);

sdk_size_t sdk_hashtable_numcol(sdk_hashtable_t *htable);

bool sdk_hashtable_contains(sdk_hashtable_t *htable, const void *k);

sdk_err_t sdk_hashtable_put(sdk_hashtable_t *htable, const void *k, const void *v);

void* sdk_hashtable_get(sdk_hashtable_t * htable, const void* key);

sdk_err_t sdk_hashtable_remove(sdk_hashtable_t * htable, const void* key);

void sdk_hashtable_foreach(sdk_hashtable_t * htable, void (*apply)(sdk_hashtable_node_t * node, void* ud), void* ud);

////////////////////////////////////////////////////////////////////////////////
////
extern sdk_hashtable_key_ops_t sdk_hashtable_string_key_ops;
extern sdk_hashtable_val_ops_t sdk_hashtable_string_val_ops;

extern sdk_hashtable_val_ops_t sdk_hashtable_ptr_val_ops;



#endif /*INCLUDED_SDK_HASHTABLE_H*/
