#ifndef INCLUDED_SDK_STRINGBUFFER_H
#define INCLUDED_SDK_STRINGBUFFER_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_BYTEORDER_H
#include <sdk_byteorder.h>
#endif /*INCLUDED_SDK_BYTEORDER_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_stringbuffer_s{
    unsigned char*      buffer;
    sdk_size_t          buffer_size;
    sdk_size_t          write_idx;
    sdk_size_t          read_idx;
}sdk_stringbuffer_t;


////////////////////////////////////////////////////////////////////////////////
////

#define SDK_STRINGBUFFER_OK                 (0x00)
#define SDK_STRINGBUFFER_FULL               (-0xE0)
#define SDK_STRINGBUFFER_EMPTY              (-0xE1)
#define SDK_STRINGBUFFER_OUTOFBOUNDARY      (-0xE2)
#define SDK_STRINGBUFFER_EINVAL             (-0xE3)

#define SDK_STRINGBUFFER_EVALUE (0xFFU)
#define SDK_STRINGBUFFER_EINDEX (-1U)

#define SDK_STRINGBUFFER_READ_U32_EVAL ((uint32_t)(0xFFFFFFFFU))
#define SDK_STRINGBUFFER_READ_U16_EVAL ((uint16_t)(0xFFFFU))

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_STRINGBUFFER_SIZE(S)     (((S)->write_idx >= (S)->read_idx) \
    ?((S)->write_idx - (S)->read_idx)                                   \
    :((S)->buffer_size - (S)->read_idx + (S)->write_idx))

#define SDK_STRINGBUFFER_CAPACITY(S) (((S)->buffer_size-1) - SDK_STRINGBUFFER_SIZE(S))

#define SDK_STRINGBUFFER_AVAILABLE(S) (SDK_STRINGBUFFER_CAPACITY(S)-SDK_STRINGBUFFER_SIZE(S))

////////////////////////////////////////////////////////////////////////////////
////

int sdk_stringbuffer_init(sdk_stringbuffer_t* sbuf, unsigned char* buffer, sdk_size_t buffer_size);
int sdk_stringbuffer_put(sdk_stringbuffer_t* sbuf, char item);
int sdk_stringbuffer_pop(sdk_stringbuffer_t* sbuf, char* item);
int sdk_stringbuffer_peek(sdk_stringbuffer_t* sbuf, char* item);

char sdk_stringbuffer_get(sdk_stringbuffer_t* sbuf, sdk_size_t idx);

sdk_size_t sdk_stringbuffer_size(sdk_stringbuffer_t* sbuf);
sdk_size_t sdk_stringbuffer_capacity(sdk_stringbuffer_t* sbuf);

/* 往前移动读指针. 移动前检查是否会越过 write_idx
 * return:
 *      SDK_STRINGBUFFER_EINVAL:             当 idx<0 || idx >=sbuf->buffer_size
 *      SDK_STRINGBUFFER_OUTOFBOUNDARY:      当 idx >= SDK_STRINGBUFFER_SIZE(sbuf)
 * */
int sdk_stringbuffer_advance_read_idx(sdk_stringbuffer_t * sbuf, sdk_size_t idx);


////////////////////////////////////////////////////////////////////////////////
//// Advance Functions
sdk_size_t sdk_stringbuffer_find(sdk_stringbuffer_t * sbuf, sdk_size_t idx, const char* needle, sdk_size_t needle_size);

unsigned long sdk_stringbuffer_strtoul(sdk_stringbuffer_t * sbuf, sdk_size_t * endptr, register int base);

uint32_t sdk_stringbuffer_read_u32(sdk_stringbuffer_t* sbuf, sdk_size_t idx, sdk_byteorder_t byteorder);
uint16_t sdk_stringbuffer_read_u16(sdk_stringbuffer_t* sbuf, sdk_size_t idx, sdk_byteorder_t byteorder);

int sdk_stringbuffer_put_u32(sdk_stringbuffer_t* sbuf, uint32_t value, sdk_byteorder_t byteorder);
int sdk_stringbuffer_put_u16(sdk_stringbuffer_t* sbuf, uint16_t value, sdk_byteorder_t byteorder);

#endif /* INCLUDED_SDK_STRINGBUFFER_H */
