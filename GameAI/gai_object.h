#ifndef INCLUDED_GAI_OBJECT_H
#define INCLUDED_GAI_OBJECT_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_GAI_DEFINITIONS_H
#include <gai_definitions.h>
#endif /*INCLUDED_GAI_DEFINITIONS_H*/

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef enum gai_object_type_enum{
    kGAI_ObjectType_Action=0,
    kGAI_ObjectType_Evaluator,
    kGAI_ObjectType_DecisionTree,
    kGAI_ObjectType_DecisionTreeNode,
    kGAI_ObjectType_FSM_State,
    kGAI_ObjectType_FSM_Transition,
    kGAI_ObjectType_FSM_Machine,
    kGAI_ObjectType_BehaviorTreeNode,
    kGAI_ObjectType_BehaviorTree,
    
}gai_object_type_t;

typedef struct gai_object_s{
    char                name[GAI_OBJECT_NAME_SIZE];
    gai_object_type_t   type;
    void*               userdata;
}gai_object_t;


////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_object_init(gai_object_t* object, const char* name, gai_object_type_t type, void* userdata);

#endif /* INCLUDED_GAI_OBJECT_H */
