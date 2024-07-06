#ifndef INCLUDED_GAI_EVALUATOR_H
#define INCLUDED_GAI_EVALUATOR_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef struct gai_evaluator_s gai_evaluator_t;

typedef int (*gai_evaluator_function_t)(gai_evaluator_t*);

struct gai_evaluator_s{
    gai_object_t object;
    gai_evaluator_function_t function;
};

////////////////////////////////////////////////////////////////////////////////
////

int gai_evaluator_init(gai_evaluator_t* evaluator, const char* name, gai_evaluator_function_t function, void* userdata);

int gai_evaluator_evaluate(gai_evaluator_t* evaluator);

#endif /* INCLUDED_GAI_EVALUATOR_H */
