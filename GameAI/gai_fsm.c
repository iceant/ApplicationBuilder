#include <gai_fsm.h>
#include <string.h>
#include "sdk_macros.h"

////////////////////////////////////////////////////////////////////////////////
////

static const char* gai_fsm__evaluate_transitions(gai_fsm_t* self, sdk_list_t* transition_list, void* ud){
    sdk_list_node_t * node;
    for(node = SDK_LIST_NEXT(transition_list); node!=transition_list; node= SDK_LIST_NEXT(node)){
        gai_fsm_transition_t * t = SDK_LIST_DATA(node, gai_fsm_transition_t, list_node);
        if(t->evaluator){
            if(gai_evaluator_evaluate(t->evaluator, ud)){
                return t->to_state_name;
            }
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//// STATE Functions

gai_err_t gai_fsm_state_init(gai_fsm_state_t* state, const char* name, gai_action_t* action, void* userdata){
    if(!state){
        return GAI_EINVAL;
    }
    
    state->action = action;
    SDK_LIST_INIT(&state->transition_list);
    SDK_LIST_INIT(&state->list_node);
    
    gai_object_init(&state->object, name, kGAI_ObjectType_FSM_State, userdata);
    return GAI_OK;
}

gai_err_t gai_fsm_state_add_transition(gai_fsm_state_t* self, gai_fsm_transition_t* transition){
    SDK_LIST_REMOVE(&transition->list_node);
    SDK_LIST_INSERT_AFTER(&self->transition_list, &transition->list_node);
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_fsm_transition_init(gai_fsm_transition_t* transition, const char* name
        , gai_evaluator_t* evaluator
        , const char* to_state_name
        , void* userdata)
{
    if(!transition){
        return GAI_EINVAL;
    }
    
    gai_object_init(&transition->object, name, kGAI_ObjectType_FSM_Transition, userdata);
    transition->evaluator = evaluator;
    if(to_state_name!=0){
        sdk_size_t name_size = strlen(to_state_name);
        name_size = SDK_MIN(SDK_ARRAY_SIZE(transition->to_state_name)-1, name_size);
        memcpy(transition->to_state_name, to_state_name, name_size);
        transition->to_state_name[name_size]='\0';
    }
    SDK_LIST_INIT(&transition->list_node);
    
    return GAI_OK;
}
////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_fsm_init(gai_fsm_t* fsm, const char* name, void* userdata)
{
    if(!fsm){
        return GAI_EINVAL;
    }
    
    gai_object_init(&fsm->object, name, kGAI_ObjectType_FSM_Machine, userdata);

    fsm->current_state = 0;
    SDK_LIST_INIT(&fsm->state_list);
    
    return GAI_OK;
}

gai_err_t gai_fsm_add_state(gai_fsm_t* self, gai_fsm_state_t * state, const char* name, gai_action_t * action, void* userdata)
{
    gai_err_t  err = gai_fsm_state_init(state, name, action, userdata);
    if(err!=GAI_OK){
        return err;
    }
    SDK_LIST_INSERT_AFTER(&self->state_list, &state->list_node);
    return GAI_OK;
}

gai_err_t gai_fsm_add_transition(gai_fsm_t* self, gai_fsm_transition_t* transition
                                 , const char* transitionName
                                 , const char* fromStateName, const char* toStateName, gai_evaluator_t * evaluator
                                 , void* userdata)
{
    if(!self || !transition || !fromStateName || !toStateName){
        return GAI_EINVAL;
    }
    
    gai_fsm_state_t * from_state = gai_fsm_find_state(self, fromStateName);
    if(from_state==0){
        return GAI_ENOTEXIST;
    }
    gai_fsm_transition_init(transition, transitionName, evaluator, toStateName, userdata);
    gai_fsm_state_add_transition(from_state, transition);
    
    transition->evaluator = evaluator;
    
    return GAI_OK;
}

gai_fsm_state_t * gai_fsm_find_state(gai_fsm_t* self, const char* state_name)
{
    sdk_list_node_t * p;
    for(p= SDK_LIST_HEAD(&self->state_list); p!=&self->state_list; p= SDK_LIST_NEXT(p)){
        gai_fsm_state_t * state = SDK_LIST_DATA(p, gai_fsm_state_t, list_node);
        if(strcmp(state->object.name, state_name)==0){
            return state;
        }
    }
    return 0;
}

sdk_bool_t gai_fsm_contains_state(gai_fsm_t * self, const char* state_name)
{
    return (gai_fsm_find_state(self, state_name)==0)?SDK_FALSE:SDK_TRUE;
}

gai_fsm_transition_t * gai_fsm_find_transition(gai_fsm_t* self, const char* fromStateName, const char* toStateName)
{
    gai_fsm_state_t * from_state = gai_fsm_find_state(self, fromStateName);
    if(from_state==0){
        return 0;
    }
    
    sdk_list_node_t * p;
    for(p= SDK_LIST_HEAD(&from_state->transition_list); p!=&from_state->transition_list; p= SDK_LIST_NEXT(p)){
        gai_fsm_transition_t * transition = SDK_LIST_DATA(p, gai_fsm_state_t, list_node);
        if(strcmp(transition->to_state_name, toStateName)==0){
            return transition;
        }
    }
    return 0;
}

sdk_bool_t gai_fsm_contains_transition(gai_fsm_t* self, const char* fromStateName, const char* toStateName)
{
    return gai_fsm_find_transition(self, fromStateName, toStateName)!=0?SDK_TRUE:SDK_FALSE;
}

gai_err_t gai_fsm_update(gai_fsm_t * self, void* ud){
    if(self->current_state && self->current_state->action){
        int status = gai_fsm_get_current_status(self);
        if(status==GAI_ACTION_STATUS_RUNNING){
            gai_action_update(self->current_state->action, ud);
        }else if(status==GAI_ACTION_STATUS_TERMINATED){
            /* evaluate all transitions to find the next state */
            sdk_list_node_t * n;
            for(n = SDK_LIST_HEAD(&self->state_list); n!=&self->state_list; n= SDK_LIST_NEXT(n)){
                gai_fsm_state_t * state = SDK_LIST_DATA(n, gai_fsm_state_t, list_node);
                const char* toStateName =gai_fsm__evaluate_transitions(self, &state->transition_list, ud);
                if(toStateName!=0){
                    gai_fsm_state_t * to_state = gai_fsm_find_state(self, toStateName);
                    if(to_state){
                        gai_action_cleanup(self->current_state->action,ud);
                        self->current_state = to_state;
                        gai_action_initialize(self->current_state->action, ud);
                        return GAI_OK;
                    }
                }
            }
        }
    }
    
    return GAI_ENOTEXIST;
}

int gai_fsm_get_current_status(gai_fsm_t* self)
{
    if(self->current_state && self->current_state->action){
        return self->current_state->action->status;
    }
    return GAI_ACTION_STATUS_UNKNOWN;
}

gai_err_t gai_fsm_set_state(gai_fsm_t * self, const char* stateName, void* ud){
    if(!self){
        return GAI_EINVAL;
    }
    
    gai_fsm_state_t * state = gai_fsm_find_state(self, stateName);
    if(!state){
        return GAI_ENOTEXIST;
    }
    self->current_state = state;
    if(self->current_state->action){
        gai_action_initialize(self->current_state->action, ud);
    }
    
}
