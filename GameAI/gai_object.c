#include <gai_object.h>
#include <string.h>
#include <sdk_macros.h>

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_object_init(gai_object_t* object, const char* name, gai_object_type_t type, void* userdata)
{
    if(object==0) return GAI_EINVAL;
    if(name!=0){
        sdk_size_t name_size = strlen(name);
        name_size = SDK_MIN(SDK_ARRAY_SIZE(object->name)-1, name_size);
        memcpy(object->name, name, name_size);
        object->name[name_size]='\0';
    }
    
    object->type = type;
    object->userdata = userdata;
    
    return GAI_OK;
}

