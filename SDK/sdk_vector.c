#include <sdk_vector.h>
#include <sdk_memory.h>
#include <assert.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
////

#define ELEMENT_SIZE sizeof(void*)

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_vector_init(sdk_vector_t* vector, sdk_size_t capacity)
{
    if(!vector) return SDK_VECTOR_EINVAL;
    vector->capacity = capacity;
    if(capacity>0){
        vector->items = SDK_ALLOC(ELEMENT_SIZE * capacity);
        if(!vector->items){
            return SDK_VECTOR_NOMEM;
        }
    }else{
        vector->items = 0;
    }
    vector->size = 0;
    return SDK_VECTOR_OK;
}

void sdk_vector_destroy(sdk_vector_t* vector){
    if(!vector) return;
    SDK_FREE(vector->items);
}

sdk_err_t sdk_vector_add(sdk_vector_t *vector, void* item)
{
    if(!vector) return SDK_VECTOR_EINVAL;
    
    if(vector->size == vector->capacity){
        return SDK_VECTOR_FULL;
    }
    
    vector->items[vector->size++] = item;
    
    return SDK_VECTOR_OK;
}

void* sdk_vector_get(sdk_vector_t * vector, sdk_size_t idx)
{
    if(!vector) return 0;
    if(idx>=vector->size) return 0;
    
    return vector->items[idx];
}

sdk_err_t sdk_vector_set(sdk_vector_t* vector, sdk_size_t idx, void* item)
{
    if(!vector) return SDK_VECTOR_EINVAL;
    if(idx>=vector->size) return SDK_VECTOR_EINVAL;
    
    vector->items[idx] = item;
    
    return SDK_VECTOR_OK;
    
}

sdk_err_t sdk_vector_resize(sdk_vector_t* vector, sdk_size_t new_capacity){
    if(!vector) return SDK_VECTOR_EINVAL;
    
    if(new_capacity==0){
        SDK_FREE(vector->items);
        vector->size = 0;
    }else if(vector->capacity==0){
        vector->items = SDK_ALLOC(ELEMENT_SIZE * new_capacity);
    }else{
        void** items = SDK_ALLOC(ELEMENT_SIZE * new_capacity);
        memcpy(items, vector->items, ELEMENT_SIZE * vector->size);
        if(vector->size > new_capacity){
            vector->size = new_capacity;
        }
        SDK_FREE(vector->items);
        vector->items = items;
    }
    
    vector->capacity = new_capacity;
    
    return SDK_VECTOR_OK;
}

sdk_err_t sdk_vector_resize_add(sdk_vector_t* vector, void* item)
{
    assert(vector);
    
    sdk_err_t err=SDK_VECTOR_OK;
    
    if(vector->size == vector->capacity){
        err = sdk_vector_resize(vector, (vector->capacity==0)?1:(vector->capacity*2));
        if(err!=SDK_VECTOR_OK){
            return err;
        }
    }
    
    vector->items[vector->size++] = item;
    
    return SDK_VECTOR_OK;
}

/*
  [0] [1] [2] [3] [4] [5]
           ^
           |- remove
 */
sdk_err_t sdk_vector_remove(sdk_vector_t* vector, sdk_size_t idx)
{
    if(!vector) return SDK_VECTOR_EINVAL;
    if(idx>=vector->size){
        return SDK_VECTOR_EINVAL;
    }
    sdk_size_t i;
    for(i=idx; i<vector->size; i++){
        vector->items[i] = NULL;
    }
    vector->items[i] = NULL;
    vector->size--;
    return SDK_VECTOR_OK;
}

sdk_err_t sdk_vector_resize_remove(sdk_vector_t * vector, sdk_size_t idx)
{
    if(!vector) return SDK_VECTOR_EINVAL;
    if(idx>=vector->size){
        return SDK_VECTOR_EINVAL;
    }
    
    sdk_size_t i;
    for(i=idx; i<vector->size; i++){
        vector->items[i] = NULL;
    }
    vector->items[i] = NULL;
    --vector->size;
    
    if(vector->size <= vector->capacity/4){
        return sdk_vector_resize(vector, vector->capacity/2);
    }
    
    return SDK_VECTOR_OK;
}

