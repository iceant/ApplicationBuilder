#ifndef INCLUDED_SDK_OBJECT_H
#define INCLUDED_SDK_OBJECT_H

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

typedef enum sdk_object_type_enum{
    kSDK_ObjectType_Thread=0,
}sdk_object_type_t;

typedef struct sdk_object_s{
    sdk_object_type_t type;
    char name[SDK_OBJECT_NAME_SIZE];
    void* userdata;
}sdk_object_t;

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_object_init(sdk_object_t* object, const char* name, sdk_object_type_t type, void* userdata);

#endif /* INCLUDED_SDK_OBJECT_H */
