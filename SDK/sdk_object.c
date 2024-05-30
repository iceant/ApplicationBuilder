#include <sdk_object.h>
#include <string.h>
#include <sdk_macros.h>

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_object_init(sdk_object_t* object, const char* name, sdk_object_type_t type, void* userdata)
{
    if(!object){
        return SDK_EINVAL;
    }
    
    if(name){
        sdk_size_t name_size = strlen(name);
        name_size = SDK_MIN(name_size, SDK_ARRAY_SIZE(object->name));
        memcpy(object->name, name, name_size);
    }
    
    object->type = type;
    
    object->userdata = userdata;
    
    return SDK_EOK;
}
