#ifndef INCLUDED_SDK_VALUE_H
#define INCLUDED_SDK_VALUE_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef struct sdk_value_s sdk_value_t;

typedef enum sdk_value_type_enum{
    kSDK_ValueType_Char=0,
    kSDK_ValueType_Integer,
    kSDK_ValueType_Double,
    kSDK_ValueType_Boolean,
    kSDK_ValueType_String,
    kSDK_ValueType_Array,
    kSDK_ValueType_Struct,
}sdk_value_type_t;

typedef struct sdk_array_spec_s{
    sdk_value_type_t    value_type; /* kSDK_ValueType_Array */
    sdk_value_type_t    item_type;
    sdk_size_t          array_size;
}sdk_array_spec_t;

typedef struct sdk_struct_spec_s{
    sdk_value_type_t    value_type; /* kSDK_ValueType_Struct */
    int                 struct_type;
    sdk_size_t          array_size;
}sdk_struct_spec_t;

typedef struct sdk_value_spec_s{
    union {
        sdk_value_type_t value_type;
        sdk_array_spec_t array_type;
        sdk_struct_spec_t struct_type;
    }type;
}sdk_value_spec_t;

struct sdk_value_s{
    sdk_value_spec_t spec;
    union {
        char    char_value;
        int     integer_value;
        double  double_value;
        bool    bool_value;
        char*   string_value;
        void*   array_value;
        void*   struct_value;
    }value;
};


////////////////////////////////////////////////////////////////////////////////
////




#endif /*INCLUDED_SDK_VALUE_H*/
