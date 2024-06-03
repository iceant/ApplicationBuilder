#ifndef INCLUDED_SDK_SCOPEGUARD_H
#define INCLUDED_SDK_SCOPEGUARD_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/



////////////////////////////////////////////////////////////////////////////////
////
typedef void (*sdk_scopeguard_free_t)(void*);
typedef struct sdk_scopeguard_node_s sdk_scopeguard_node_t;
typedef struct sdk_scopeguard_s sdk_scopeguard_t;

struct sdk_scopeguard_node_s{
    sdk_scopeguard_node_t * next;
    sdk_scopeguard_free_t free;
    void* data;
};

struct sdk_scopeguard_s{
    sdk_bool_t is_dismissed;
    sdk_scopeguard_node_t * list;
};

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_SCOPEGUARD_EOK          SDK_EOK
#define SDK_SCOPEGUARD_ERROR        SDK_ERROR
#define SDK_SCOPEGUARD_ENOMEM       SDK_ENOMEM


////////////////////////////////////////////////////////////////////////////////
////
sdk_err_t sdk_scopeguard_init(sdk_scopeguard_t * guard);

void sdk_scopeguard_destroy(sdk_scopeguard_t * guard);

sdk_err_t sdk_scopeguard_on_exit(sdk_scopeguard_t * guard, sdk_scopeguard_free_t fn, void* data);

void sdk_scopeguard_set_dismiss(sdk_scopeguard_t * guard, sdk_bool_t is_dismiss);

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_SCOPE_BEGIN(n) \
do{                          \
    sdk_scopeguard_t __scope__##n; \
    sdk_scopeguard_init(&__scope__##n);


#define SDK_SCOPE_ONEXIT(n, fn, d) \
    sdk_scopeguard_on_exit(&__scope__##n, (sdk_scopeguard_free_t)fn, (d))

#define SDK_SCOPE_DISMISS(n) \
    sdk_scopeguard_set_dismiss(&__scope__##n, SDK_TRUE)

#define SDK_SCOPE_END(n) \
    sdk_scopeguard_destroy(&__scope__##n); \
}while(0);


#endif /*INCLUDED_SDK_SCOPEGUARD_H*/
