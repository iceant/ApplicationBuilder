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
    sdk_size_t   buffer_size;
    sdk_size_t   object_size;
    sdk_size_t   read_idx;
    sdk_size_t   write_idx;
}sdk_ringbuffer_t;

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_RINGBUFFER_OK           (0x00 )
#define SDK_RINGBUFFER_FULL         (-0x10)
#define SDK_RINGBUFFER_EMPTY        (-0x11)
#define SDK_RINGBUFFER_EBUFSIZE     (-0x12)
#define SDK_RINGBUFFER_EINDEX       (-0x13)

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_RINGBUFFER_SIZE(B)     (((B)->write_idx >= (B)->read_idx) \
    ?((B)->write_idx - (B)->read_idx)                                 \
    :((B)->buffer_size - (B)->read_idx + (B)->write_idx))

#define SDK_RINGBUFFER_CAPACITY(S) (((S)->buffer_size-(S)->object_size)/(S)->object_size)

#define SDK_RINGBUFFER_AVAILABLE(S) (SDK_RINGBUFFER_CAPACITY(S)-SDK_RINGBUFFER_SIZE(S))

#define SDK_RINGBUFFER_IS_EMPTY(B) ((B)->read_idx==(B)->write_idx)

#define SDK_RINGBUFFER_IS_FULL(B) (SDK_RINGBUFFER_AVAILABLE(B)==0)

////////////////////////////////////////////////////////////////////////////////
////

int sdk_ringbuffer_init(sdk_ringbuffer_t* rb, void* buffer, sdk_size_t buffer_size, sdk_size_t item_size);

/* contained how many items */
sdk_size_t sdk_ringbuffer_size(sdk_ringbuffer_t * rb);

int sdk_ringbuffer_put(sdk_ringbuffer_t* rb, const void* item);

int sdk_ringbuffer_pop(sdk_ringbuffer_t* rb, void* item);

void* sdk_ringbuffer_put_slot(sdk_ringbuffer_t* rb);

int sdk_ringbuffer_peek(sdk_ringbuffer_t * rb, sdk_size_t idx, void* item);

#endif /* INCLUDED_SDK_RINGBUFFER_H */
