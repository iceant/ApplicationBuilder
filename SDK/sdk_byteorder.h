#ifndef INCLUDED_SDK_BYTEORDER_H
#define INCLUDED_SDK_BYTEORDER_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef enum sdk_byteorder_enum{
    kSDK_ByteOrder_LittleEndian=0,
    kSDK_ByteOrder_BigEndian=1
}sdk_byteorder_t;


#endif /*INCLUDED_SDK_BYTEORDER_H*/
