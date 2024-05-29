#ifndef INCLUDED_GAI_BEHAVIORTREE_H
#define INCLUDED_GAI_BEHAVIORTREE_H

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

#ifndef INCLUDED_SDK_LIST_H
#include <sdk_list.h>
#endif /*INCLUDED_SDK_LIST_H*/



////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_behavior_tree_node_s gai_behavior_tree_node_t;
typedef struct gai_behavior_tree_s gai_behavior_tree_t;

typedef enum gai_behavior_tree_node_type_enum{
    kGAI_BehaviorTreeNodeType_Action=0,
    kGAI_BehaviorTreeNodeType_Condition,
    kGAI_BehaviorTreeNodeType_Selector,
    kGAI_BehaviorTreeNodeType_Sequence,
}gai_behavior_tree_node_type_t;

struct gai_behavior_tree_node_s{
    gai_object_t object;
    sdk_list_t children_list;
    sdk_list_node_t child_node;
    gai_action_t * action;
    gai_evaluator_t * evaluator;
    gai_behavior_tree_node_t * parent;
    gai_behavior_tree_node_type_t type;
};

struct gai_behavior_tree_s{
    gai_object_t object;
    gai_behavior_tree_node_t * current_node;
    gai_behavior_tree_node_t * root;
};

////////////////////////////////////////////////////////////////////////////////
//// Node Functions

gai_err_t gai_behavior_tree_node_init(gai_behavior_tree_node_t* node, const char* name
                                      , gai_behavior_tree_node_type_t type, void* userdata);

gai_err_t gai_behavior_tree_node_add_child(gai_behavior_tree_node_t* self, gai_behavior_tree_node_t* child);
gai_err_t gai_behavior_tree_node_set_action(gai_behavior_tree_node_t* self, gai_action_t* action);
gai_err_t gai_behavior_tree_node_set_evaluator(gai_behavior_tree_node_t* self, gai_evaluator_t * evaluator);
gai_err_t gai_behavior_tree_node_set_type(gai_behavior_tree_node_t* self, gai_behavior_tree_node_type_t type);

////////////////////////////////////////////////////////////////////////////////
//// Tree Functions
gai_err_t gai_behavior_tree_init(gai_behavior_tree_t* self, const char * name, void* userdata);

gai_err_t gai_behavior_tree_update(gai_behavior_tree_t* tree, void* ud);

gai_err_t gai_behavior_tree_set_root(gai_behavior_tree_t* tree, gai_behavior_tree_node_t * root);

////////////////////////////////////////////////////////////////////////////////
//// UTILS
gai_err_t gai_behavior_tree_node_action(gai_behavior_tree_node_t* node, const char* name, gai_action_t * action, void* userdata);
gai_err_t gai_behavior_tree_node_condition(gai_behavior_tree_node_t* node, const char* name, gai_evaluator_t* evaluator, void* userdata);
gai_err_t gai_behavior_tree_node_sequence(gai_behavior_tree_node_t* node, const char* name, void* userdata);
gai_err_t gai_behavior_tree_node_selector(gai_behavior_tree_node_t* node, const char* name, void* userdata);

#endif /* INCLUDED_GAI_BEHAVIORTREE_H */
