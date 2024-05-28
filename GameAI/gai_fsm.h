#ifndef INCLUDED_GAI_FSM_H
#define INCLUDED_GAI_FSM_H

#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

#ifndef INCLUDED_SDK_LIST_H
#include <sdk_list.h>
#endif /*INCLUDED_SDK_LIST_H*/



////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_fsm_s gai_fsm_t;
typedef struct gai_fsm_state_s gai_fsm_state_t;
typedef struct gai_fsm_transition_s gai_fsm_transition_t;

struct gai_fsm_state_s{
    gai_object_t object;
    gai_action_t * action;
    sdk_list_t transition_list;
    sdk_list_node_t list_node;
};

struct gai_fsm_transition_s{
    gai_object_t object;
    gai_evaluator_t * evaluator;
    char to_state_name[GAI_OBJECT_NAME_SIZE];
    sdk_list_node_t list_node;
};


struct gai_fsm_s{
    gai_object_t object;
    gai_fsm_state_t * current_state;
    sdk_list_t state_list;
};

////////////////////////////////////////////////////////////////////////////////
////
gai_err_t gai_fsm_state_init(gai_fsm_state_t* state, const char* name, gai_action_t* action, void* userdata);

gai_err_t gai_fsm_state_add_transition(gai_fsm_state_t* state, gai_fsm_transition_t* transition);
////////////////////////////////////////////////////////////////////////////////
////
gai_err_t gai_fsm_transition_init(gai_fsm_transition_t* transition, const char* name
                                  , gai_evaluator_t* evaluator
                                  , const char* to_state_name
                                  , void* userdata);

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_fsm_init(gai_fsm_t* fsm, const char* name, void* userdata);

gai_err_t gai_fsm_add_transition(gai_fsm_t* self, gai_fsm_transition_t* transition
        , const char* transitionName
        , const char* fromStateName, const char* toStateName, gai_evaluator_t * evaluator
        , void* userdata);

gai_err_t gai_fsm_add_state(gai_fsm_t* self, gai_fsm_state_t * state
                            , const char* name, gai_action_t * action, void* userdata);

sdk_bool_t gai_fsm_contains_transition(gai_fsm_t* self, const char* fromStateName, const char* toStateName);

gai_fsm_transition_t * gai_fsm_find_transition(gai_fsm_t* self, const char* fromStateName, const char* toStateName);

sdk_bool_t gai_fsm_contains_state(gai_fsm_t * self, const char* state_name);

gai_fsm_state_t * gai_fsm_find_state(gai_fsm_t* self, const char* state_name);

int gai_fsm_get_current_status(gai_fsm_t* self);

gai_err_t gai_fsm_update(gai_fsm_t * self, void* ud);

gai_err_t gai_fsm_set_state(gai_fsm_t * self, const char* stateName, void* ud);

#endif /* INCLUDED_GAI_FSM_H */
