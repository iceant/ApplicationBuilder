#include <gai_decisiontree.h>

////////////////////////////////////////////////////////////////////////////////
////
gai_err_t gai_decision_tree_node_init(gai_decision_tree_node_t* node, const char* name, gai_decision_tree_node_type_t type, void* userdata)
{
    if(!node) return GAI_EINVAL;
    
    gai_object_init(&node->object, name, kGAI_ObjectType_DecisionTreeNode, userdata);
    
    node->type = type;
    sdk_vector_init(&node->children, 0);
    
    node->evaluator = 0;
    node->action = 0;
    
    return GAI_OK;
}

gai_err_t gai_decision_tree_node_add_child(gai_decision_tree_node_t* node, gai_decision_tree_node_t * child){
    sdk_err_t err = sdk_vector_resize_add(&node->children, child);
    if(err!=SDK_VECTOR_OK){
        return GAI_ERROR;
    }
    return GAI_OK;
}

gai_decision_tree_node_t * gai_decision_tree_node_evaluate(gai_decision_tree_node_t* node, void* ud){
    int child_idx = gai_evaluator_evaluate(node->evaluator, ud);
    
    if(child_idx==GAI_EINVAL || child_idx==GAI_EVALUATOR_NOFUNCTION) return 0;
    
    gai_decision_tree_node_t * choice = sdk_vector_get(&node->children, child_idx);
    if(choice->type==kGAI_DecisionTreeNodeType_Branch){
        return gai_decision_tree_node_evaluate(choice, ud);
    }
    return choice;
}

void gai_decision_tree_node_destroy(gai_decision_tree_node_t* node){
    if(!node) return;
    sdk_vector_destroy(&node->children);
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


