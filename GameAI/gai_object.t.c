#include <gai_object.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    gai_object_t gaiObject;
    gai_object_init(&gaiObject, kGAI_ObjectType_Action, "Action-1", 0);
    printf("%s\n", gaiObject.name);
    printf("\ttype:%d\n", gaiObject.type);
    return 0;
}
