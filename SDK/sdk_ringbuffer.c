#include <sdk_ringbuffer.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
////

int sdk_ringbuffer_init(sdk_ringbuffer_t* rb, void* buffer, void* buffer_end, sdk_size_t item_size)
{
    rb->buffer = buffer;
    rb->buffer_end = buffer_end;
    rb->item_size = item_size;
    rb->write_p = rb->buffer;
    rb->read_p = rb->buffer;
    return SDK_RINGBUFFER_OK;
}

sdk_size_t sdk_ringbuffer_length(sdk_ringbuffer_t * rb)
{
    return SDK_RINGBUFFER_LENGTH(rb);
}

int sdk_ringbuffer_put(sdk_ringbuffer_t* rb, const void* item)
{
    void* next_write_p = rb->write_p + rb->item_size;
    if(next_write_p >= rb->buffer_end){
        next_write_p = rb->buffer;
    }
    if(next_write_p == rb->read_p){
        return SDK_RINGBUFFER_FULL;
    }
    
    memcpy(rb->write_p, item, rb->item_size);
    
    rb->write_p = next_write_p;
    
    return SDK_RINGBUFFER_OK;
}

int sdk_ringbuffer_pop(sdk_ringbuffer_t* rb, void* item)
{
    if(SDK_RINGBUFFER_IS_EMPTY(rb)){
        return SDK_RINGBUFFER_EMPTY;
    }
    
    memcpy(item, rb->read_p, rb->item_size);
    
    void* next_read_p = rb->read_p + rb->item_size;
    if(next_read_p>=rb->buffer_end){
        next_read_p= rb->buffer;
    }
    rb->read_p = next_read_p;
    
    return SDK_RINGBUFFER_OK;
}

int sdk_ringbuffer_peek(sdk_ringbuffer_t* rb, void* item)
{
    if(SDK_RINGBUFFER_IS_EMPTY(rb)){
        return SDK_RINGBUFFER_EMPTY;
    }
    
    memcpy(item, rb->read_p, rb->item_size);
    
    return SDK_RINGBUFFER_OK;
}

