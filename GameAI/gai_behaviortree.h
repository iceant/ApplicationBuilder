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
typedef enum gai_behavior_tree_node_type_enum{
    kGAI_BehaviorTreeNodeType_Action = 0,
    kGAI_BehaviorTreeNodeType_Condition,
    kGAI_BehaviorTreeNodeType_Selector,
    kGAI_BehaviorTreeNodeType_Sequence,
}gai_behavior_tree_node_type_t;

typedef struct gai_behavior_tree_node_s{
    gai_behavior_tree_node_type_t type;
    char name[GAI_OBJECT_NAME_SIZE];
    gai_action_t* action_p;
    gai_evaluator_t * evaluator_p;
    sdk_list_t children_list;
    sdk_list_node_t node;
    struct gai_behavior_tree_node_s* parent_p;
}gai_behavior_tree_node_t;


typedef struct gai_behavior_tree_s{
    gai_behavior_tree_node_t* root;
    gai_behavior_tree_node_t* current_node;
    void* userdata;
}gai_behavior_tree_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_behavior_tree_node_init(gai_behavior_tree_node_t * node, gai_behavior_tree_node_type_t type, const char* name);

int gai_behavior_tree_node_add_child(gai_behavior_tree_node_t* self, gai_behavior_tree_node_t * child);

////////////////////////////////////////////////////////////////////////////////
////

int gai_behavior_tree_action_init(gai_behavior_tree_node_t * node, const char* name, gai_action_t* action);

int gai_behavior_tree_condition_init(gai_behavior_tree_node_t * node, const char* name, gai_evaluator_t* evaluator);

int gai_behavior_tree_selector_init(gai_behavior_tree_node_t * node, const char* name);

int gai_behavior_tree_sequence_init(gai_behavior_tree_node_t * node, const char* name);

////////////////////////////////////////////////////////////////////////////////
////

int gai_behavior_tree_init(gai_behavior_tree_t* tree, void* userdata);

int gai_behavior_tree_update(gai_behavior_tree_t* tree, void* userdata);

#endif /* INCLUDED_GAI_BEHAVIORTREE_H */
