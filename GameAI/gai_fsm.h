#ifndef INCLUDED_GAI_FSM_H
#define INCLUDED_GAI_FSM_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_LIST_H
#include <sdk_list.h>
#endif /*INCLUDED_SDK_LIST_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef struct gai_fsm_state_s gai_fsm_state_t;
typedef struct gai_fsm_transition_s gai_fsm_transition_t;

struct gai_fsm_state_s{
    char name[GAI_OBJECT_NAME_SIZE];
    gai_action_t action;
    sdk_list_t transition_list;
    sdk_list_node_t node;
};

struct gai_fsm_transition_s{
    char to_state_name[GAI_OBJECT_NAME_SIZE];
    sdk_list_node_t node;
    gai_evaluator_t evaluator;
};

typedef struct gai_fsm_s{
    gai_object_t object;
    gai_fsm_state_t * current_state;
    sdk_list_t state_list;
}gai_fsm_t;
////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_state_init(gai_fsm_state_t* state, const char* name
        , gai_action_function_t initialize_function
        , gai_action_function_t update_function
        , gai_action_function_t cleanup_function
        , void* userdata);

int gai_fsm_state_add_transition(gai_fsm_state_t* state, gai_fsm_transition_t* transition);

int gai_fsm_state_find_transition(gai_fsm_state_t* state, const char* to_state_name
                                     , gai_fsm_transition_t** result);
////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_transition_init(gai_fsm_transition_t* transition, const char* to_state_name, gai_evaluator_function_t evaluator_function, void* userdata);

////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_init(gai_fsm_t* fsm, const char* name, void* userdata);

int gai_fsm_add_state(gai_fsm_t * fsmt, gai_fsm_state_t* state);

int gai_fsm_find_transition(gai_fsm_t* fsm, const char* from_state, const char* to_state, gai_fsm_transition_t** result);

int gai_fsm_find_state(gai_fsm_t* fsm, const char* state_name, gai_fsm_state_t ** result);

int gai_fsm_add_transition(gai_fsm_t * fsm, const char* state_name, gai_fsm_transition_t* transition);

int gai_fsm_set_state(gai_fsm_t* fsm, gai_fsm_state_t * state);

int gai_fsm_set_state_by_name(gai_fsm_t* fsm, const char* state_name);

int gai_fsm_update(gai_fsm_t * fsm);

#endif /* INCLUDED_GAI_FSM_H */
