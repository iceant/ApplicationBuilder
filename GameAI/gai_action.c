#include <gai_action.h>

int gai_action_init(gai_action_t* action, const char* name
        , gai_action_function_t initialize_function
        , gai_action_function_t update_function
        , gai_action_function_t cleanup_function
        , void* userdata)
{
    gai_object_init(&action->object, kGAI_ObjectType_Action, name, userdata);
    action->initialize_function = initialize_function;
    action->update_function = update_function;
    action->cleanup_function = cleanup_function;
    action->status = kGAI_ActionStatus_UNINITIALIZED;
    
    return GAI_OK;
}

int gai_action_initialize(gai_action_t* action){
    if(!action) return GAI_EINVAL;
    if(action->status==kGAI_ActionStatus_UNINITIALIZED){
        if(action->initialize_function){
            action->initialize_function(action);
        }
        action->status = kGAI_ActionStatus_RUNNING;
    }
    return GAI_OK;
}

gai_action_status_t gai_action_update(gai_action_t * action){
    if(action->status == kGAI_ActionStatus_TERMINATED){
        /* Immediately return if the Action has already terminated. */
        return kGAI_ActionStatus_TERMINATED;
    }else if(action->status==kGAI_ActionStatus_RUNNING){
        if(action->update_function){
            action->status = action->update_function(action);
        }else{
            action->status = kGAI_ActionStatus_TERMINATED;
        }
    }
    return action->status;
}

int gai_action_cleanup(gai_action_t * action){
    if(!action) return GAI_EINVAL;
    if(action->status==kGAI_ActionStatus_TERMINATED){
        if(action->cleanup_function){
            action->cleanup_function(action);
        }
    }
    action->status = kGAI_ActionStatus_UNINITIALIZED;
    return GAI_OK;
}

