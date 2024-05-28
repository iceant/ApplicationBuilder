#ifndef INCLUDED_SDK_RINGBUFFER_H
#define INCLUDED_SDK_RINGBUFFER_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_ringbuffer_s{
    void *       buffer;
    void *       buffer_end;
    sdk_size_t  item_size;
    void *       read_p;
    void *       write_p;
}sdk_ringbuffer_t;

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_RINGBUFFER_OK     (0x00 )
#define SDK_RINGBUFFER_FULL   (-0x10)
#define SDK_RINGBUFFER_EMPTY  (-0x11)

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_RINGBUFFER_LENGTH(RB) ((uint8_t*)(RB)->buffer_end - (uint8_t*)(RB)->buffer)

#define SDK_RINGBUFFER_IS_EMPTY(RB) ((RB)->read_p==(RB)->write_p)

#define SDK_RINGBUFFER_CAPACITY(RB) (((char*)(RB)->buffer_end - (char*)(RB)->buffer)/(RB)->item_size)

////////////////////////////////////////////////////////////////////////////////
////

int sdk_ringbuffer_init(sdk_ringbuffer_t* rb, void* buffer, void* buffer_end, sdk_size_t item_size);

sdk_size_t sdk_ringbuffer_length(sdk_ringbuffer_t * rb);

int sdk_ringbuffer_put(sdk_ringbuffer_t* rb, const void* item);

int sdk_ringbuffer_pop(sdk_ringbuffer_t* rb, void* item);


#endif /* INCLUDED_SDK_RINGBUFFER_H */
