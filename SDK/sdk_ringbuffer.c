#include <sdk_ringbuffer.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

#define CHECK_INDEX(R, I, ACTION) \
    if((R)->read_idx == (R)->write_idx){ \
        ACTION                    \
    }                             \
    sdk_size_t capacity = SDK_RINGBUFFER_CAPACITY((R)); \
    if((I)>capacity){  \
        ACTION                    \
    }                             \
    sdk_size_t size = SDK_RINGBUFFER_SIZE((R));         \
    if( (I) > size){              \
        ACTION                    \
    }


////////////////////////////////////////////////////////////////////////////////
////
int sdk_ringbuffer_init(sdk_ringbuffer_t* rb, void* buffer, sdk_size_t buffer_size, sdk_size_t item_size)
{
    if((buffer_size-item_size) < item_size){
        return SDK_RINGBUFFER_EBUFSIZE;
    }

    rb->buffer = buffer;
    rb->object_size = item_size;
    rb->buffer_size = buffer_size;
    rb->write_idx = 0;
    rb->read_idx = 0;

    return SDK_RINGBUFFER_OK;
}

sdk_size_t sdk_ringbuffer_size(sdk_ringbuffer_t * rb)
{
    return SDK_RINGBUFFER_SIZE(rb);
}

int sdk_ringbuffer_put(sdk_ringbuffer_t* rb, const void* item)
{
    sdk_size_t capacity = SDK_RINGBUFFER_CAPACITY(rb);

    sdk_size_t next_write_idx = rb->write_idx+1;
    if(next_write_idx > capacity){
        next_write_idx = 0;
    }

    if(next_write_idx==rb->read_idx){
        return SDK_RINGBUFFER_FULL;
    }

    memcpy(rb->buffer+rb->write_idx*rb->object_size, item, rb->object_size);

    rb->write_idx = next_write_idx;

    return SDK_RINGBUFFER_OK;
}

int sdk_ringbuffer_pop(sdk_ringbuffer_t* rb, void* item)
{
    if(SDK_RINGBUFFER_IS_EMPTY(rb)){
        return SDK_RINGBUFFER_EMPTY;
    }

    sdk_size_t capacity = SDK_RINGBUFFER_CAPACITY(rb);
    sdk_size_t next_read_idx = rb->read_idx+1;
    if(next_read_idx>capacity){
        next_read_idx = 0;
    }

    memcpy(item, rb->buffer+rb->read_idx*rb->object_size, rb->object_size);
    rb->read_idx = next_read_idx;

    return SDK_RINGBUFFER_OK;
}

void* sdk_ringbuffer_put_slot(sdk_ringbuffer_t* rb)
{
    sdk_size_t capacity = SDK_RINGBUFFER_CAPACITY(rb);

    sdk_size_t next_write_idx = rb->write_idx+1;
    if(next_write_idx>capacity){
        next_write_idx = 0;
    }

    if(next_write_idx==rb->read_idx){
        /*BUFFER IS FULL!!!*/
        return 0;
    }

    void* result = rb->buffer+rb->write_idx*rb->object_size;
    rb->write_idx = next_write_idx;
    return result;
}



int sdk_ringbuffer_peek(sdk_ringbuffer_t * rb, sdk_size_t idx, void* item){

    CHECK_INDEX(rb, idx, return SDK_RINGBUFFER_EINDEX;)

//    /* 确保有数据 */
//    if(rb->read_idx == rb->write_idx){
//        return SDK_RINGBUFFER_EINDEX;
//    }
//
//    /* idx 在容量范围内 */
//    sdk_size_t capacity = SDK_RINGBUFFER_CAPACITY(rb);
//    if(idx>capacity){
//        return SDK_RINGBUFFER_EINDEX;
//    }
//
//    /* idx 在已有数据中 */
//    sdk_size_t size = SDK_RINGBUFFER_SIZE(rb);
//    if( idx > size){
//        return SDK_RINGBUFFER_EINDEX;
//    }

    sdk_size_t next_read_idx = rb->read_idx + idx;
    if(next_read_idx>capacity){
        next_read_idx =  next_read_idx - capacity;
    }

    memcpy(item, rb->buffer + next_read_idx * rb->object_size, rb->object_size);

    return SDK_RINGBUFFER_OK;
}
