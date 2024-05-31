#ifndef INCLUDED_SDK_MD5_H
#define INCLUDED_SDK_MD5_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////

/* Unlike previous versions of this code, uint32 need not be exactly
   32 bits, merely 32 bits or more.  Choosing a data type which is 32
   bits instead of 64 is not important; speed is considerably more
   important.  ANSI guarantees that "unsigned long" will be big enough,
   and always using it seems to have few disadvantages.  */

typedef struct sdk_md5_context_s {
    uint32_t buf[4];
    uint32_t bits[2];
    unsigned char in[64];
}sdk_md5_context_t;

////////////////////////////////////////////////////////////////////////////////
////


void sdk_md5_init(sdk_md5_context_t *context);
void sdk_md5_update(sdk_md5_context_t *context, unsigned char const *buf, unsigned len);
void sdk_md5_final(unsigned char digest[16], sdk_md5_context_t *context);
void sdk_md5_transform(uint32_t buf[4], const unsigned char in[64]);


////////////////////////////////////////////////////////////////////////////////
////

void sdk_md5_string(char *input, uint8_t result[16]);

#endif /*INCLUDED_SDK_MD5_H*/
