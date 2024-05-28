#ifndef INCLUDED_SDK_STRINGBUFFER_H
#define INCLUDED_SDK_STRINGBUFFER_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

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

#define SDK_STRINGBUFFER_OK         (0x00)
#define SDK_STRINGBUFFER_FULL       (-0xE0)
#define SDK_STRINGBUFFER_EMPTY      (-0xE1)

#define SDK_RINGBUFFER_INVALID_VALUE (-1U)
////////////////////////////////////////////////////////////////////////////////
////

int sdk_stringbuffer_init(sdk_stringbuffer_t* sbuf, unsigned char* buffer, sdk_size_t buffer_size);
int sdk_stringbuffer_put(sdk_stringbuffer_t* sbuf, char item);
int sdk_stringbuffer_pop(sdk_stringbuffer_t* sbuf, char* item);
int sdk_stringbuffer_peek(sdk_stringbuffer_t* sbuf, char* item);

char sdk_stringbuffer_get(sdk_stringbuffer_t* sbuf);
char sdk_stringbuffer_try_get(sdk_stringbuffer_t* sbuf);

#endif /* INCLUDED_SDK_STRINGBUFFER_H */
