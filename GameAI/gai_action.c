#include <gai_action.h>

int gai_action_init(gai_action_t* action, const char* name
        , gai_action_function_t initialize_function
        , gai_action_function_t update_function
        , gai_action_function_t cleanup_function
        , void* userdata)
{
    if(!action) return GAI_EINVAL;
    
    gai_object_init(&action->object, name, kGAI_ObjectType_Action, userdata);
    
    action->status = GAI_ACTION_STATUS_UNINITIALIZED;
    action->initialize_function = initialize_function;
    action->update_function = update_function;
    action->cleanup_function = cleanup_function;
    
    return GAI_OK;
}

int gai_action_initialize(gai_action_t* action, void* userdata)
{
    if(!action) return GAI_EINVAL;
    
    if(action->status == GAI_ACTION_STATUS_UNINITIALIZED){
        if(action->initialize_function){
            action->initialize_function(action, userdata);
        }
    }
    action->status = GAI_ACTION_STATUS_RUNNING;
    return GAI_OK;
}

int gai_action_update(gai_action_t* action, void* userdata)
{
    if(!action) return GAI_EINVAL;
    
    if(action->status == GAI_ACTION_STATUS_TERMINATED){
        return GAI_ACTION_STATUS_TERMINATED;
    }else if(action->status == GAI_ACTION_STATUS_RUNNING){
        if(action->update_function){
            int status = action->update_function(action, userdata);
            if(!GAI_ACTION_STATUS_IS_VALID(status)){
                return GAI_ERETVAL;
            }
            action->status = status;
        }
    }else{
        action->status = GAI_ACTION_STATUS_TERMINATED;
    }
    
    return action->status;
}

int gai_action_cleanup(gai_action_t* action, void* userdata){
    if(!action) return GAI_EINVAL;
    
    if(action->status==GAI_ACTION_STATUS_TERMINATED){
        if(action->cleanup_function){
            action->cleanup_function(action, userdata);
        }
    }
    action->status = GAI_ACTION_STATUS_UNINITIALIZED;
    return GAI_OK;
}

