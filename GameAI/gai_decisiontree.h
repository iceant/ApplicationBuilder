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

typedef struct gai_decision_tree_s gai_decision_tree_t;
typedef struct gai_decision_tree_node_s gai_decision_tree_node_t;

typedef enum gai_decision_tree_node_type_enum{
    kGAI_DecisionTreeNodeType_Branch = 0,
    kGAI_DecisionTreeNodeType_Action,
}gai_decision_tree_node_type_t;

struct gai_decision_tree_node_s{
    gai_object_t object;
    gai_decision_tree_node_type_t type;
    sdk_vector_t children;
    gai_evaluator_t* evaluator;
    gai_action_t * action;
};

struct gai_decision_tree_s{
    gai_object_t object;
    gai_decision_tree_node_t*   branch;
    gai_decision_tree_node_t*   current_action;
};

////////////////////////////////////////////////////////////////////////////////
////

#define GAI_DECISIONTREE_ENOBRANCH (-0xD001)

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_decision_tree_node_init(gai_decision_tree_node_t* node, const char* name, gai_decision_tree_node_type_t type, void* userdata);

void gai_decision_tree_node_destroy(gai_decision_tree_node_t* node);

gai_err_t gai_decision_tree_node_add_child(gai_decision_tree_node_t* node, gai_decision_tree_node_t * child);

gai_decision_tree_node_t * gai_decision_tree_node_evaluate(gai_decision_tree_node_t* node, void* ud);

gai_err_t gai_decision_tree_node_set_evaluator(gai_decision_tree_node_t* node, gai_evaluator_t* evaluator);

////////////////////////////////////////////////////////////////////////////////
////


gai_err_t gai_decision_tree_init(gai_decision_tree_t* tree, const char* name, void* userdata);

gai_err_t gai_decision_tree_set_branch(gai_decision_tree_t* tree, gai_decision_tree_node_t* branch);

gai_err_t gai_decision_tree_update(gai_decision_tree_t *tree, void* ud);


#endif /* INCLUDED_GAI_DECISIONTREE_H */
