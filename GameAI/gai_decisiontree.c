#include <gai_decisiontree.h>

////////////////////////////////////////////////////////////////////////////////
////
gai_err_t gai_decision_tree_node_init(gai_decision_tree_node_t* node, const char* name, gai_decision_tree_node_type_t type, void* userdata)
{
    if(!node) return GAI_EINVAL;
    
    gai_object_init(&node->object, name, kGAI_ObjectType_DecisionTreeNode, userdata);
    
    node->type = type;
    SDK_LIST_INIT(&node->children_list);
    SDK_LIST_INIT(&node->child_node);

    node->evaluator = 0;
    node->action = 0;
    
    return GAI_OK;
}

gai_err_t gai_decision_tree_node_add_child(gai_decision_tree_node_t* node, gai_decision_tree_node_t * child){
    if(!node || !child){
        return GAI_EINVAL;
    }
    SDK_LIST_REMOVE(&child->child_node);
    SDK_LIST_INSERT_BEFORE(&node->children_list, &child->child_node); /*Add to TAIL*/
    return GAI_OK;
}

gai_decision_tree_node_t * gai_decision_tree_node_evaluate(gai_decision_tree_node_t* node, void* ud){
    int child_idx = gai_evaluator_evaluate(node->evaluator, ud);
    
    if(child_idx==GAI_EINVAL || child_idx==GAI_EVALUATOR_NOFUNCTION) return 0;

    sdk_list_node_t * list_node;
    gai_decision_tree_node_t * tree_node;
    int idx = 0;
    SDK_LIST_FOREACH(list_node, &node->children_list){
        tree_node = SDK_LIST_DATA(list_node, gai_decision_tree_node_t, child_node);
        if(child_idx==idx){
            break;
        }
        idx+=1;
    }

    if(idx==child_idx && list_node!=&node->children_list){
        if(tree_node->type==kGAI_DecisionTreeNodeType_Branch){
            return gai_decision_tree_node_evaluate(tree_node, ud);
        }
    }

    return tree_node;
}


gai_err_t gai_decision_tree_node_set_evaluator(gai_decision_tree_node_t* node, gai_evaluator_t* evaluator)
{
    if(!node){
        return GAI_EINVAL;
    }
    node->evaluator = evaluator;
    return GAI_OK;
}
////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_decision_tree_init(gai_decision_tree_t* tree, const char* name, void* userdata)
{
    if(!tree) return GAI_EINVAL;
    
    gai_object_init(&tree->object, name, kGAI_ObjectType_DecisionTree, userdata);
    
    return GAI_OK;
}

gai_err_t gai_decision_tree_set_branch(gai_decision_tree_t* tree, gai_decision_tree_node_t* branch)
{
    if(!tree) return GAI_EINVAL;
    
    tree->branch = branch;
    
    return GAI_OK;
}

gai_err_t gai_decision_tree_update(gai_decision_tree_t * tree, void* userdata)
{
    if(tree->branch==0){
        return GAI_DECISIONTREE_ENOBRANCH;
    }
    
    if(!tree->current_action){
        tree->current_action = gai_decision_tree_node_evaluate(tree->branch, userdata);
        if(!tree->current_action) return GAI_ERETVAL;
        gai_action_initialize(tree->current_action->action, userdata);
    }
    
    int status = gai_action_update(tree->current_action->action, userdata);
    if(status==GAI_ACTION_STATUS_TERMINATED){
        gai_action_cleanup(tree->current_action->action, userdata);
        tree->current_action = 0;
    }
    
    return GAI_OK;
}


