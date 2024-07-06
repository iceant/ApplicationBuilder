#ifndef INCLUDED_GAI_ACTION_H
#define INCLUDED_GAI_ACTION_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_action_s gai_action_t;

typedef enum gai_action_status_enum{
    kGAI_ActionStatus_UNINITIALIZED = 0,
    kGAI_ActionStatus_RUNNING = 1,
    kGAI_ActionStatus_TERMINATED = 2,
}gai_action_status_t;

typedef gai_action_status_t (*gai_action_function_t)(gai_action_t* action);

struct gai_action_s{
    gai_object_t object;
    gai_action_function_t initialize_function;
    gai_action_function_t update_function;
    gai_action_function_t cleanup_function;
    gai_action_status_t status;
};

////////////////////////////////////////////////////////////////////////////////
////

int gai_action_init(gai_action_t* action, const char* name
                    , gai_action_function_t initialize_function
                    , gai_action_function_t update_function
                    , gai_action_function_t cleanup_function
                    , void* userdata);

int gai_action_initialize(gai_action_t* action);

gai_action_status_t gai_action_update(gai_action_t * action);

int gai_action_cleanup(gai_action_t * action);


#endif /* INCLUDED_GAI_ACTION_H */
