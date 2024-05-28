#ifndef INCLUDED_GAI_EVALUATOR_H
#define INCLUDED_GAI_EVALUATOR_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/


////////////////////////////////////////////////////////////////////////////////
////
#define GAI_EVALUATOR_NOFUNCTION (-0xEFEE)

////////////////////////////////////////////////////////////////////////////////
////


typedef struct gai_evaluator_s gai_evaluator_t;
typedef int (*gai_evaluator_function_t)(gai_evaluator_t* evaluator, void* userdata);

struct gai_evaluator_s{
    gai_object_t object;
    gai_evaluator_function_t function;
};

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_evaluator_init(gai_evaluator_t * evaluator, const char* name
                             , gai_evaluator_function_t function
                             , void* userdata);

/*
    return
        - GAI_EVALUATOR_NOFUNCTION: no function
        - function result
 */
int gai_evaluator_evaluate(gai_evaluator_t* evaluator, void* userdata);


////////////////////////////////////////////////////////////////////////////////
////

extern gai_evaluator_t GAI_TRUE_Evaluator;
extern gai_evaluator_t GAI_FALSE_Evaluator;

////////////////////////////////////////////////////////////////////////////////
////

#define GAI_EVALUATOR_INIT(N, FN, UD) { \
    .object.name=N                      \
    , .object.userdata=(UD)             \
    , .object.type=kGAI_ObjectType_Evaluator \
    , .function = FN \
}

#endif /* INCLUDED_GAI_EVALUATOR_H */
