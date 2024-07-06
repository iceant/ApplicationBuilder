#ifndef INCLUDED_GAI_DECISIONTREE_H
#define INCLUDED_GAI_DECISIONTREE_H
////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_decisiontree_s gai_decisiontree_t;

typedef struct gai_decisionbranch_s{
    gai_object_t object;
    gai_evaluator_t evaluator;
    sdk_vector_t children;
}gai_decisionbranch_t;

struct gai_decisiontree_s
{
    gai_object_t object;
    gai_decisionbranch_t* branch;
    gai_action_t* current_action;
};

////////////////////////////////////////////////////////////////////////////////
////
int gai_decisiontree_init(gai_decisiontree_t* tree, const char* name, void* userdata);

int gai_decisiontree_set_branch(gai_decisiontree_t * tree, gai_decisionbranch_t * branch);

int gai_decisiontree_update(gai_decisiontree_t* tree, void* userdata);

////////////////////////////////////////////////////////////////////////////////
////

int gai_decisionbranch_init(gai_decisionbranch_t* branch, const char* name, void* userdata);

void gai_decisionbranch_destroy(gai_decisionbranch_t * branch);

int gai_decisionbranch_set_evaluator(gai_decisionbranch_t * branch, const char* name, gai_evaluator_function_t function, void* userdata);

int gai_decisionbranch_evaluate(gai_decisionbranch_t* branch, gai_action_t** result);

int gai_decisionbranch_add_child(gai_decisionbranch_t* branch, gai_object_t * child);

#endif /* INCLUDED_GAI_DECISIONTREE_H */
