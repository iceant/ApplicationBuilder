#include <sdk_scopeguard.h>
#include <assert.h>
#include "sdk_memory.h"

sdk_err_t sdk_scopeguard_init(sdk_scopeguard_t * guard)
{
    guard->is_dismissed=SDK_FALSE;
    guard->list = 0;
    return SDK_SCOPEGUARD_EOK;
}

void sdk_scopeguard_destroy(sdk_scopeguard_t * guard){
    sdk_scopeguard_node_t * p;
    if(guard->is_dismissed==SDK_FALSE){
        while(guard->list){
            p = guard->list;
            guard->list = p->next;
            if(p->free){
                p->free(p->data);
            }
            SDK_FREE(p);
        }
    }else{
        while(guard->list){
            p = guard->list;
            guard->list = p->next;
            SDK_FREE(p);
        }
    }
}

sdk_err_t sdk_scopeguard_on_exit(sdk_scopeguard_t * guard, sdk_scopeguard_free_t fn, void* data)
{
    sdk_scopeguard_node_t * node;
    SDK_NEW(node);
    if(!node){
        return SDK_SCOPEGUARD_ENOMEM;
    }
    node->free = fn;
    node->data = data;
    node->next = guard->list;
    guard->list = node;
    return SDK_SCOPEGUARD_EOK;
}

void sdk_scopeguard_set_dismiss(sdk_scopeguard_t * guard, sdk_bool_t is_dismiss)
{
    guard->is_dismissed = is_dismiss;
}

