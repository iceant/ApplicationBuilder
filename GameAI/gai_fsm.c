#include <gai_fsm.h>
#include <string.h>
#include <sdk_macros.h>
////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_state_init(gai_fsm_state_t* state, const char* name
                          , gai_action_function_t initialize_function
                          , gai_action_function_t update_function
                          , gai_action_function_t cleanup_function
                          , void* userdata)
{
    size_t name_size = strlen(name);
    memset(state->name, 0, sizeof(state->name));
    name_size = (name_size > (GAI_OBJECT_NAME_SIZE-1))?(GAI_OBJECT_NAME_SIZE-1):name_size;
    memcpy(state->name, name, name_size);
    state->name[name_size]='\0';
    
    gai_action_init(&state->action, name, initialize_function, update_function, cleanup_function, userdata);
    SDK_LIST_INIT(&state->transition_list);
    return GAI_OK;
}

int gai_fsm_state_add_transition(gai_fsm_state_t* state, gai_fsm_transition_t* transition)
{
    sdk_list_node_t *node;
    sdk_list_t * header = &state->transition_list;
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_fsm_transition_t * transition_p = SDK_CONTAINER_OF(node, gai_fsm_transition_t, node);
        if(strcmp(transition->to_state_name, transition_p->to_state_name)==0){
            return GAI_OK;
        }
    }
    SDK_LIST_INSERT_BEFORE(&state->transition_list, &transition->node);
    return GAI_OK;
}

gai_action_status_t gai_finite_state_status_get(gai_fsm_state_t* state){
    return state->action.status;
}

int gai_fsm_state_find_transition(gai_fsm_state_t* state, const char* to_state_name, gai_fsm_transition_t** result){
    sdk_list_node_t *node;
    sdk_list_t * header = &state->transition_list;
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_fsm_transition_t * transition_p = SDK_CONTAINER_OF(node, gai_fsm_transition_t, node);
        if(strcmp(to_state_name, transition_p->to_state_name)==0){
            if(result){
                *result = transition_p;
            }
            return GAI_OK;
        }
    }
    if(result){
        *result = 0;
    }
    return GAI_ERROR;
}
////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_transition_init(gai_fsm_transition_t* transition
                               , const char* to_state_name
                               , gai_evaluator_function_t evaluator_function
                               , void* userdata)
{
    size_t name_size = strlen(to_state_name);
    memset(transition->to_state_name, 0, sizeof(transition->to_state_name));
    name_size = (name_size > (GAI_OBJECT_NAME_SIZE-1))?(GAI_OBJECT_NAME_SIZE-1):name_size;
    memcpy(transition->to_state_name, to_state_name, name_size);
    transition->to_state_name[name_size]='\0';
    
    gai_evaluator_init(&transition->evaluator, to_state_name, evaluator_function, userdata);
    SDK_LIST_INIT(&transition->node);
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_fsm_init(gai_fsm_t* fsm, const char* name, void* userdata)
{
    gai_object_init(&fsm->object, kGAI_ObjectType_FSM , name, userdata);
    SDK_LIST_INIT(&fsm->state_list);
    fsm->current_state = 0;
    return GAI_OK;
}

int gai_fsm_add_state(gai_fsm_t * fsm, gai_fsm_state_t* state)
{
    sdk_list_node_t *node;
    sdk_list_t * header = &fsm->state_list;
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_fsm_state_t * node_p = SDK_CONTAINER_OF(node, gai_fsm_state_t, node);
        if(strcmp(state->name, node_p->name)==0){
            return GAI_OK;
        }
    }
    SDK_LIST_INSERT_BEFORE(&fsm->state_list, &state->node);
    return GAI_OK;
}

int gai_fsm_find_state(gai_fsm_t* fsm, const char* state_name, gai_fsm_state_t ** result){
    sdk_list_node_t *node;
    sdk_list_t * header = &fsm->state_list;
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_fsm_state_t * node_p = SDK_CONTAINER_OF(node, gai_fsm_state_t, node);
        if(strcmp(state_name, node_p->name)==0){
            if(result){
                *result = node_p;
            }
            return GAI_OK;
        }
    }
    if(result){
        *result = 0;
    }
    return GAI_ERROR;
}

int gai_fsm_find_transition(gai_fsm_t* fsm, const char* from_state, const char* to_state, gai_fsm_transition_t** result){
    gai_fsm_state_t *state = 0;
    int err = gai_fsm_find_state(fsm, from_state, &state);
    if(err!=GAI_OK){
        return GAI_ERROR;
    }
    
    err = gai_fsm_state_find_transition(state, to_state, result);
    if(err!=GAI_OK){
        return err;
    }
    
    return GAI_OK;
}

int gai_fsm_add_transition(gai_fsm_t * fsm, const char* state_name, gai_fsm_transition_t* transition)
{
    gai_fsm_state_t * state = 0;
    int err = gai_fsm_find_state(fsm, state_name, &state);
    if(err!=GAI_OK){
        return err;
    }
    
    return gai_fsm_state_add_transition(state, transition);
}

int gai_fsm_evaluate_transitions(gai_fsm_t* fsm, sdk_list_t* transitions, char** result)
{
    sdk_list_node_t *node;
    sdk_list_t * header = transitions;
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_fsm_transition_t * node_p = SDK_CONTAINER_OF(node, gai_fsm_transition_t, node);
        int err = gai_evaluator_evaluate(&node_p->evaluator);
        if(err!=GAI_ERROR && err>0){
            if(result){
                *result = node_p->to_state_name;
            }
            return GAI_OK;
        }
    }
    
    return GAI_ERROR;
}


int gai_fsm_update(gai_fsm_t * fsm){
    if(fsm->current_state){
        gai_action_status_t status = fsm->current_state->action.status;
        if(status==kGAI_ActionStatus_RUNNING){
            gai_action_update(&fsm->current_state->action);
            return GAI_OK;
        }else if(status==kGAI_ActionStatus_TERMINATED){
            char* to_state_name = 0;
            int err = gai_fsm_evaluate_transitions(fsm, &fsm->current_state->transition_list, &to_state_name);
            if(err!=GAI_OK){
                gai_action_cleanup(&fsm->current_state->action);
                return err;
            }
            gai_fsm_state_t * next_state=0;
            err = gai_fsm_find_state(fsm, to_state_name, &next_state);
            if(err!=GAI_OK){
                return err;
            }
            
            gai_action_cleanup(&fsm->current_state->action);
            gai_fsm_set_state(fsm, next_state);
            gai_action_initialize(&fsm->current_state->action);
            return GAI_OK;
        }
    }
    return GAI_OK;
}

int gai_fsm_set_state(gai_fsm_t* fsm, gai_fsm_state_t * state){
    if(fsm->current_state){
        gai_action_cleanup(&fsm->current_state->action);
    }
    fsm->current_state = state;
    gai_action_initialize(&fsm->current_state->action);
    return GAI_OK;
}

int gai_fsm_set_state_by_name(gai_fsm_t* fsm, const char* state_name)
{
    gai_fsm_state_t *state = 0;
    int err = gai_fsm_find_state(fsm, state_name, &state);
    if(err!=GAI_OK){
        return err;
    }
    gai_fsm_set_state(fsm, state);
    return GAI_OK;
}
