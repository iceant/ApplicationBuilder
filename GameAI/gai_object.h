#ifndef INCLUDED_GAI_OBJECT_H
#define INCLUDED_GAI_OBJECT_H


////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_DEFINITIONS_H
#include <gai_definitions.h>
#endif /*INCLUDED_GAI_DEFINITIONS_H*/

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef enum gai_object_type_enum{
    kGAI_ObjectType_Action = 1,
    kGAI_ObjectType_Evaluator=2,
    kGAI_ObjectType_DecisionBranch=3,
    kGAI_ObjectType_DecisionTree=4,
}gai_object_type_t;

typedef struct gai_object_s{
    gai_object_type_t type;
    char name[GAI_OBJECT_NAME_SIZE];
    void* userdata;
}gai_object_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_object_init(gai_object_t* object, gai_object_type_t type, const char* name, void* userdata);

#endif /* INCLUDED_GAI_OBJECT_H */
