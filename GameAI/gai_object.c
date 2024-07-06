#include <gai_object.h>
#include <string.h>

int gai_object_init(gai_object_t* object, gai_object_type_t type, const char* name, void* userdata)
{
    object->type = type;
    size_t name_size = strlen(name);
    memset(object->name, 0, sizeof(object->name));
    name_size = (name_size > (GAI_OBJECT_NAME_SIZE-1))?(GAI_OBJECT_NAME_SIZE-1):name_size;
    memcpy(object->name, name, name_size);
    object->name[name_size]='\0';
    object->userdata = userdata;
    
    return GAI_OK;
}
