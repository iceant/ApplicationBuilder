#ifndef INCLUDED_GAI_ACTION_H
#define INCLUDED_GAI_ACTION_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/


////////////////////////////////////////////////////////////////////////////////
////

#define GAI_ACTION_STATUS_RUNNING           1
#define GAI_ACTION_STATUS_TERMINATED        2
#define GAI_ACTION_STATUS_UNINITIALIZED     4

#define GAI_ACTION_STATUS_IS_VALID(S) \
    ((S)==GAI_ACTION_STATUS_RUNNING  \
    || (S)==GAI_ACTION_STATUS_TERMINATED \
    || (S)==GAI_ACTION_STATUS_UNINITIALIZED)


////////////////////////////////////////////////////////////////////////////////
////
typedef struct gai_action_s gai_action_t;

typedef int (*gai_action_function_t)(gai_action_t* action, void* userdata);

struct gai_action_s{
    gai_object_t object;
    int status;
    gai_action_function_t initialize_function;
    gai_action_function_t update_function; /* return status of action */
    gai_action_function_t cleanup_function;
};

////////////////////////////////////////////////////////////////////////////////
////

int gai_action_init(gai_action_t* action, const char* name
                    , gai_action_function_t initialize_function
                    , gai_action_function_t update_function
                    , gai_action_function_t cleanup_function
                    , void* userdata);

int gai_action_initialize(gai_action_t* action, void* userdata);

/*
 * return
 *      - action.status if success
 *      - GAI_EINVAL if action is NULL
 */
int gai_action_update(gai_action_t* action, void* userdata);


int gai_action_cleanup(gai_action_t* action, void* userdata);


#endif /* INCLUDED_GAI_ACTION_H */
