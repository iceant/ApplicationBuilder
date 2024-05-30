#include <sdk_hashtable.h>
#include <assert.h>
#include <sdk_memory.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
////

static sdk_vector_t * sdk_hashtable__new_bucket(void){
    sdk_vector_t * bucket = SDK_ALLOC(sizeof(*bucket));
    if(!bucket) return 0;
    sdk_vector_init(bucket, 0);
    return bucket;
}

static sdk_hashtable_node_t * sdk_hashtable__get_node(sdk_hashtable_t *htable, const void *key) {

    sdk_hashtable_node_t *result = NULL;
    sdk_hashtable_node_t *crt_node = NULL;
    sdk_vector_t *crt_bucket = NULL;

    sdk_size_t i;
    uint32_t computed_hash;
    sdk_size_t bucket_idx;

    computed_hash = htable->key_ops.hash(key, htable->key_ops.arg);
    bucket_idx = computed_hash % htable->capacity;
    crt_bucket = htable->buckets[bucket_idx];

    if(crt_bucket==0) return 0;

    for(i = 0; i < crt_bucket->size; i++) {
        crt_node = crt_bucket->items[i];
        if (crt_node->hash == computed_hash) {
            if (htable->key_ops.eq(crt_node->key, key, htable->key_ops.arg)) {
                result = crt_node;
                break;
            }
        }
    }
    return result;
}

static sdk_err_t sdk_hashtable__resize(sdk_hashtable_t *htable, sdk_size_t new_capacity) {


    sdk_vector_t **new_buckets;
    sdk_vector_t *crt_bucket;
    sdk_hashtable_node_t *crt_element;

    sdk_size_t new_idx;
    sdk_size_t i;

//    new_capacity = htable->capacity * 2;
    new_buckets = SDK_ALLOC(sizeof(*new_buckets) * new_capacity);

    if (NULL==new_buckets) {
        return SDK_ENOMEM;
    }

    for(i = 0; i < new_capacity; ++i) {
        new_buckets[i] = 0;
    }

    // Rehash
    // For each (old) bucket
    for(i = 0; i < htable->capacity; i++) {
        crt_bucket = htable->buckets[i];
        // For each element from the old bucket
        if(!crt_bucket) continue;
        for(int j = 0; j < crt_bucket->size; j++) {
            // For each element from the
            crt_element = crt_bucket->items[j];
            // Compute the new id for the new bucket
            new_idx = crt_element->hash % new_capacity;
            // If the bucket doesn't exist yet, we create yet
            if (NULL==new_buckets[new_idx]) {
                sdk_vector_t * bucket = sdk_hashtable__new_bucket();
                if(!bucket){
                    return SDK_ENOMEM;
                }
                new_buckets[new_idx] = bucket;
            }
            // Add the element to the corresponding bucket
            sdk_vector_resize_add(new_buckets[new_idx], crt_element);
        }
    }

    htable->capacity = new_capacity;

    // Free the old buckets
    SDK_FREE(htable->buckets);

    // Update with the new buckets
    htable->buckets = new_buckets;

    return SDK_EOK;
}


static sdk_size_t sdk_hashtable__bucket_numcol(sdk_vector_t * bucket) {
    if(!bucket) return 0;
    return (bucket->size == 0) ? 0 : bucket->size-1;
}


////////////////////////////////////////////////////////////////////////////////
////


sdk_err_t sdk_hashtable_init(sdk_hashtable_t * htable, sdk_size_t capacity, sdk_hashtable_key_ops_t key_ops, sdk_hashtable_val_ops_t val_ops)
{
    assert(htable);
    assert(capacity>0);

    htable->capacity = capacity;
    htable->key_ops = key_ops;
    htable->val_ops = val_ops;
    htable->size = 0;

    htable->buckets = SDK_ALLOC(sizeof(*(htable->buckets)) * capacity);
    if(!htable->buckets){
        return SDK_ENOMEM;
    }

    for(sdk_size_t i=0; i<capacity; i++){
        htable->buckets[i] = 0;
    }

    return SDK_EOK;
}


void sdk_hashtable_destroy(sdk_hashtable_t * htable){
    assert(htable);
    sdk_vector_t * bucket;
    sdk_hashtable_node_t* node;
    sdk_size_t i;
    sdk_size_t j;
    for(i=0; i<htable->capacity; i++){
        bucket = htable->buckets[i];
        if(!bucket) continue;
        for(j=0; j<bucket->size; j++){
            node = bucket->items[j];
            htable->key_ops.free(node->key, htable->key_ops.arg);
            htable->val_ops.free(node->value, htable->val_ops.arg);
            SDK_FREE(node);
        }
        sdk_vector_destroy(bucket);
    }
    SDK_FREE(htable->buckets);
}

void* sdk_hashtable_get(sdk_hashtable_t * htable, const void* key){
    sdk_hashtable_node_t * result = sdk_hashtable__get_node(htable, key);
    if(!result){
        return 0;
    }
    return result->value;
}


sdk_err_t sdk_hashtable_put(sdk_hashtable_t *htable, const void *k, const void *v) {

    sdk_hashtable_node_t *crt;
    sdk_size_t bucket_idx;

    crt = sdk_hashtable__get_node(htable, k);

    if (crt) {
        // Key already exists
        // We need to update the value
        htable->val_ops.free(crt->value, htable->val_ops.arg);
        crt->value = v ? htable->val_ops.cp(v, htable->val_ops.arg) : 0;
    } else {
        // Key doesn't exist
        // - We create a node
        // - We add a node to the correspoding bucket
        SDK_NEW(crt);
        if (NULL == crt) {
            return SDK_ENOMEM;
        }
        crt->hash = htable->key_ops.hash(k, htable->key_ops.arg);
        crt->key = htable->key_ops.cp(k, htable->key_ops.arg);
        crt->value = htable->val_ops.cp(v, htable->val_ops.arg);

        bucket_idx = crt->hash % htable->capacity;
        if (NULL==htable->buckets[bucket_idx]) {
            htable->buckets[bucket_idx] = sdk_hashtable__new_bucket();
            if(!htable->buckets[bucket_idx]){
                return SDK_ENOMEM;
            }
        }
        sdk_vector_resize_add(htable->buckets[bucket_idx], crt);

        // Element has been added successfully
        htable->size++;

        // Grow if needed
        if (htable->size >= htable->capacity * 2) {
            sdk_err_t  err = sdk_hashtable__resize(htable, htable->capacity * 2);
            if(err!=SDK_EOK){
                return err;
            }
        }
    }
    return SDK_EOK;
}

bool sdk_hashtable_contains(sdk_hashtable_t *htable, const void *k) {
    return sdk_hashtable__get_node(htable, k) ? true : false;
}

sdk_size_t sdk_hashtable_numcol(sdk_hashtable_t *htable) {
    sdk_size_t result = 0;
    for(int i = 0; i < htable->capacity; ++i) {
        result += sdk_hashtable__bucket_numcol(htable->buckets[i]);
    }
    return result;
}

void sdk_hashtable_foreach(sdk_hashtable_t * htable, void (*apply)(sdk_hashtable_node_t * node, void* ud), void* ud)
{
    assert(htable);
    if(!apply) return;

    sdk_hashtable_node_t *crt_node = 0;
    sdk_vector_t *crt_bucket = 0;

    sdk_size_t i;
    sdk_size_t j;

    for(i=0; i<htable->capacity; i++){
        crt_bucket = htable->buckets[i];
        if(!crt_bucket) continue;
        for(j=0; j<crt_bucket->size; j++){
            crt_node = crt_bucket->items[j];
            apply(crt_node, ud);
        }
    }
}

sdk_err_t sdk_hashtable_remove(sdk_hashtable_t * htable, const void* key)
{
    assert(htable);
    assert(key);

    sdk_hashtable_node_t *crt_node = 0;
    sdk_vector_t *crt_bucket = 0;

    sdk_size_t i;
    uint32_t computed_hash;
    sdk_size_t bucket_idx;

    computed_hash = htable->key_ops.hash(key, htable->key_ops.arg);
    bucket_idx = computed_hash % htable->capacity;
    crt_bucket = htable->buckets[bucket_idx];

    if(!crt_bucket) return SDK_ENONE;

    for(i = 0; i < crt_bucket->size; ++i) {
        crt_node = crt_bucket->items[i];
        if (crt_node->hash == computed_hash) {
            if (htable->key_ops.eq(crt_node->key, key, htable->key_ops.arg)) {
                sdk_vector_resize_remove(crt_bucket, i);
                htable->size--;
                break;
            }
        }
    }

    if(htable->size == htable->capacity/4){
        sdk_err_t  err = sdk_hashtable__resize(htable, htable->capacity/2);
        if(err!=SDK_EOK){
            return err;
        }
    }

    return SDK_EOK;
}


////////////////////////////////////////////////////////////////////////////////
////
static uint32_t sdk_hash_fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x3243f6a9U;
    h ^= h >> 16;
    return h;
}

uint32_t sdk_string_hash(const void *data, void *arg) {
    //djb2
    uint32_t hash = (const uint32_t) 5381;
    const char *str = (const char*) data;
    char c;
    while((c=*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return sdk_hash_fmix32(hash);
}

void* sdk_string_cp(const void *data, void *arg) {
    const char *input = (const char*) data;
    size_t input_length = strlen(input) + 1;
    char *result;
    result = SDK_ALLOC(sizeof(*result) * input_length);
    if (NULL==result) {
        return 0;
    }
    strcpy(result, input);
    return result;
}

bool sdk_string_eq(const void *data1, const void *data2, void *arg) {
    const char *str1 = (const char*) data1;
    const char *str2 = (const char*) data2;
    return !(strcmp(str1, str2)) ? true : false;
}

void sdk_string_free(void *data, void *arg) {
    SDK_FREE(data);
}

sdk_hashtable_key_ops_t sdk_hashtable_string_key_ops={.hash=sdk_string_hash, .cp=sdk_string_cp, .free=sdk_string_free, .eq=sdk_string_eq, .arg=0};
sdk_hashtable_val_ops_t sdk_hashtable_string_val_ops={.cp=sdk_string_cp, .free=sdk_string_free, .eq=sdk_string_eq, .arg=0};


