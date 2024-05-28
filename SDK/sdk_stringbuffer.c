#include <sdk_stringbuffer.h>

int sdk_stringbuffer_init(sdk_stringbuffer_t* sbuf, unsigned char* buffer, sdk_size_t buffer_size){
    sbuf->buffer = buffer;
    sbuf->buffer_size = buffer_size;
    sbuf->write_idx = 0;
    sbuf->read_idx = 0;
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_put(sdk_stringbuffer_t* sbuf, char item)
{
    sdk_size_t next_write_idx = sbuf->write_idx + 1;
    if(next_write_idx>=sbuf->buffer_size){
        next_write_idx = 0;
    }
    if(next_write_idx == sbuf->read_idx){
        return SDK_STRINGBUFFER_FULL;
    }
    
    sbuf->buffer[sbuf->write_idx] = item;
    sbuf->write_idx= next_write_idx;
    
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_pop(sdk_stringbuffer_t* sbuf, char* item)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_EMPTY;
    }
    
    sdk_size_t next_read_idx = sbuf->read_idx + 1;
    if(next_read_idx>=sbuf->buffer_size){
        next_read_idx = 0;
    }
    
    *item = (char)sbuf->buffer[sbuf->read_idx];
    
    sbuf->read_idx = next_read_idx;
    
    return SDK_STRINGBUFFER_OK;
}

int sdk_stringbuffer_peek(sdk_stringbuffer_t* sbuf, char* item)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_STRINGBUFFER_EMPTY;
    }

    *item = (char)sbuf->buffer[sbuf->read_idx];
    
    return SDK_STRINGBUFFER_OK;
}

char sdk_stringbuffer_get(sdk_stringbuffer_t* sbuf)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_RINGBUFFER_INVALID_VALUE;
    }
    
    sdk_size_t next_read_idx = sbuf->read_idx + 1;
    if(next_read_idx>=sbuf->buffer_size){
        next_read_idx = 0;
    }
    
    char ch = (char)sbuf->buffer[sbuf->read_idx];
    
    sbuf->read_idx = next_read_idx;
    
    return ch;
}

char sdk_stringbuffer_try_get(sdk_stringbuffer_t* sbuf)
{
    if(sbuf->read_idx == sbuf->write_idx){
        return SDK_RINGBUFFER_INVALID_VALUE;
    }
    return (char)sbuf->buffer[sbuf->read_idx];
}