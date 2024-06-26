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

#define SDK_RINGBUFFER_OK               (0x00 )
#define SDK_RINGBUFFER_FULL             (-16)
#define SDK_RINGBUFFER_EMPTY            (-17)
#define SDK_RINGBUFFER_EBUFSIZE         (-18)
#define SDK_RINGBUFFER_EINDEX           (-19)
#define SDK_RINGBUFFER_EINVAL           (-20)

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_RINGBUFFER_SIZE(B)     (((B)->write_idx >= (B)->read_idx) \
    ?((B)->write_idx - (B)->read_idx)                                 \
    :(((B)->buffer_size/(B)->object_size) - (B)->read_idx + (B)->write_idx))

#define SDK_RINGBUFFER_CAPACITY(S) (((S)->buffer_size - (S)->object_size)/(S)->object_size)

#define SDK_RINGBUFFER_AVAILABLE(S) (SDK_RINGBUFFER_CAPACITY(S)-SDK_RINGBUFFER_SIZE(S))

#define SDK_RINGBUFFER_IS_EMPTY(B) ((B)->read_idx==(B)->write_idx)

#define SDK_RINGBUFFER_IS_FULL(B) (SDK_RINGBUFFER_AVAILABLE(B)==0)

////////////////////////////////////////////////////////////////////////////////
////

/*
 * return
 *      SDK_RINGBUFFER_EBUFSIZE: if (buffer_size-item_size) < item_size
 *      SDK_RINGBUFFER_OK: everything is fine!
 */
int sdk_ringbuffer_init(sdk_ringbuffer_t* rb, void* buffer, sdk_size_t buffer_size, sdk_size_t item_size);

/* contained how many items */
sdk_size_t sdk_ringbuffer_size(sdk_ringbuffer_t * rb);

int sdk_ringbuffer_put(sdk_ringbuffer_t* rb, const void* item);

int sdk_ringbuffer_pop(sdk_ringbuffer_t* rb, void* item);

void* sdk_ringbuffer_put_slot(sdk_ringbuffer_t* rb);

int sdk_ringbuffer_peek(sdk_ringbuffer_t * rb, sdk_size_t idx, void* item);


/* 往前移动读指针. 移动前检查是否会越过 write_idx
 * return:
 *      SDK_RINGBUFFER_EINVAL:             当 idx<0 || idx >=sbuf->buffer_size
 *      SDK_RINGBUFFER_OUTOFBOUNDARY:      当 idx >= SDK_RINGBUFFER_SIZE(sbuf)
 * */
int sdk_ringbuffer_advance_read_idx(sdk_ringbuffer_t * rb, sdk_size_t idx);


#endif /* INCLUDED_SDK_RINGBUFFER_H */
