#include <gai_behaviortree.h>

gai_err_t gai_behavior_tree_node_init(gai_behavior_tree_node_t* node, const char* name, gai_behavior_tree_node_type_t type, void* userdata)
{
    if(!node){
        return GAI_EINVAL;
    }
    
    gai_object_init(&node->object, name, kGAI_ObjectType_BehaviorTreeNode, userdata);
    node->action = 0;
    node->evaluator = 0;
    node->parent = 0;
    node->type = type;
    
    SDK_LIST_INIT(&node->children_list);
    SDK_LIST_INIT(&node->child_node);
    
    return GAI_OK;
}

gai_err_t gai_behavior_tree_node_add_child(gai_behavior_tree_node_t* self, gai_behavior_tree_node_t* child){
    if(!self || !child){
        return GAI_EINVAL;
    }
    
    SDK_LIST_REMOVE(&child->child_node);
    SDK_LIST_INSERT_BEFORE(&self->children_list, &child->child_node);
    child->parent = self;
    
    return GAI_OK;
}

gai_err_t gai_behavior_tree_node_set_action(gai_behavior_tree_node_t* self, gai_action_t* action){
    if(!self){
        return GAI_EINVAL;
    }
    self->action = action;
    return GAI_OK;
}

gai_err_t gai_behavior_tree_node_set_evaluator(gai_behavior_tree_node_t* self, gai_evaluator_t * evaluator){
    if(!self){
        return GAI_EINVAL;
    }
    self->evaluator = evaluator;
    return GAI_OK;
}

gai_err_t gai_behavior_tree_node_set_type(gai_behavior_tree_node_t* self, gai_behavior_tree_node_type_t type){
    if(!self){
        return GAI_EINVAL;
    }
    self->type = type;
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

gai_err_t gai_behavior_tree_node_action(gai_behavior_tree_node_t* node, const char* name, gai_action_t * action, void* userdata)
{
    gai_err_t err = gai_behavior_tree_node_init(node, name, kGAI_BehaviorTreeNodeType_Action, userdata);
    if(err!=GAI_OK){
        return err;
    }
    node->action = action;
    return err;
}

gai_err_t gai_behavior_tree_node_condition(gai_behavior_tree_node_t* node, const char* name, gai_evaluator_t * evaluator, void* userdata)
{
    gai_err_t err = gai_behavior_tree_node_init(node, name, kGAI_BehaviorTreeNodeType_Condition, userdata);
    if(err!=GAI_OK){
        return err;
    }
    node->evaluator = evaluator;
    return err;
}

gai_err_t gai_behavior_tree_node_sequence(gai_behavior_tree_node_t* node, const char* name, void* userdata)
{
    return gai_behavior_tree_node_init(node, name, kGAI_BehaviorTreeNodeType_Sequence, userdata);
}

gai_err_t gai_behavior_tree_node_selector(gai_behavior_tree_node_t* node, const char* name, void* userdata)
{
    return gai_behavior_tree_node_init(node, name, kGAI_BehaviorTreeNodeType_Selector, userdata);
}
////////////////////////////////////////////////////////////////////////////////
//// Tree Functions

gai_err_t gai_behavior_tree_init(gai_behavior_tree_t* self, const char * name, void* userdata)
{
    if(!self){
        return GAI_EINVAL;
    }
    
    gai_object_init(&self->object, name, kGAI_ObjectType_BehaviorTree, userdata);
    
    self->current_node = 0;
    self->root = 0;
    
    return GAI_OK;
}

gai_err_t gai_behavior_tree_set_root(gai_behavior_tree_t* tree, gai_behavior_tree_node_t * root){
    if(!tree){
        return GAI_EINVAL;
    }
    tree->root = root;
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////
typedef struct gai_behavior_tree_evaluate_result_s{
    sdk_bool_t result;
    gai_behavior_tree_node_t * node;
}gai_behavior_tree_evaluate_result_t;

static gai_behavior_tree_evaluate_result_t* gai_behavior_tree__evaluate_sequence(gai_behavior_tree_evaluate_result_t* result
        , gai_behavior_tree_t * self
        , gai_behavior_tree_node_t * node
        , gai_behavior_tree_node_t * last_node
        , void* ud);

static gai_behavior_tree_evaluate_result_t* gai_behavior_tree__evaluate_selector(gai_behavior_tree_evaluate_result_t* result
                                                                        , gai_behavior_tree_t * self
                                                                       , gai_behavior_tree_node_t * node, void* ud)
{
    /* try and evaluate all children. return the first child can execute. */
    gai_behavior_tree_node_t * child_node;
    sdk_list_node_t * child;
    SDK_LIST_FOREACH(child, &node->children_list){
        child_node = SDK_LIST_DATA(child, gai_behavior_tree_node_t, child_node);
        if(child_node->type==kGAI_BehaviorTreeNodeType_Action){
            result->node = child_node;
            result->result = SDK_TRUE;
            return result;
        }else if(child_node->type == kGAI_BehaviorTreeNodeType_Condition){
            /* condition is invalid in selector */
            result->result = SDK_FALSE;
            return result;
        }else if(child_node->type==kGAI_BehaviorTreeNodeType_Selector){
            gai_behavior_tree__evaluate_selector(result, self, child_node, ud);
            if(result->result==SDK_TRUE){
                return result;
            }
        }else if(child_node->type==kGAI_BehaviorTreeNodeType_Sequence){
            gai_behavior_tree__evaluate_sequence(result, self, child_node, 0, ud);
            if(result->result==SDK_TRUE){
                return result;
            }
        }
    }
    result->result = false;
    return result;
}

static gai_behavior_tree_evaluate_result_t* gai_behavior_tree__evaluate_sequence(gai_behavior_tree_evaluate_result_t* result
        , gai_behavior_tree_t * self
        , gai_behavior_tree_node_t * node
        , gai_behavior_tree_node_t * start_tree_node
        , void* ud)
{
    gai_behavior_tree_node_t* child_node;
    sdk_list_node_t * child;
    sdk_list_node_t * start_list_node;
    
    if(start_tree_node==0){
        start_list_node = SDK_LIST_HEAD(&node->children_list);
    }else{
        start_list_node = &start_tree_node->child_node;
    }
    
    for(child = start_list_node; child!=&node->children_list; child = SDK_LIST_NEXT(child)){
        child_node = SDK_LIST_DATA(child, gai_behavior_tree_node_t, child_node);
        if(child_node->type == kGAI_BehaviorTreeNodeType_Action){
            result->node = child_node;
            result->result = SDK_TRUE;
            return result;
        }else if(child_node->type == kGAI_BehaviorTreeNodeType_Condition){
            if(child_node->evaluator){
                int evaluate_result = gai_evaluator_evaluate(child_node->evaluator, ud);
                if(evaluate_result!=GAI_EVALUATOR_NOFUNCTION){
                    if(evaluate_result==0/* false */){
                        result->result = SDK_FALSE;
                        return result;
                    }
                }
            }
        }else if(child_node->type == kGAI_BehaviorTreeNodeType_Selector){
            gai_behavior_tree__evaluate_selector(result, self, child_node, ud);
            if(result->result==SDK_FALSE){
                return result;
            }else if(result->result==SDK_TRUE && result->node!=0){
                return result;
            }
        }else if(child_node->type == kGAI_BehaviorTreeNodeType_Sequence){
            gai_behavior_tree__evaluate_sequence(result, self, child_node, 0/*start_tree_node*/, ud);
            if(result->result==SDK_FALSE){
                return result;
            }else if(result->result==SDK_TRUE && result->node!=0){
                return result;
            }
        }
        
    }
    
    result->result = SDK_TRUE;
    return result;
    
}

static gai_behavior_tree_node_t* gai_behavior_tree_evaluate_node(gai_behavior_tree_evaluate_result_t* result
                                                                   , gai_behavior_tree_t* self
                                                                   , gai_behavior_tree_node_t * node
                                                                   , void* ud)
{
    if(node->type==kGAI_BehaviorTreeNodeType_Action){
        return node;
    }else if(node->type == kGAI_BehaviorTreeNodeType_Selector){
        gai_behavior_tree__evaluate_selector(result, self, node, ud);
        if(result->result==SDK_TRUE){
            return result->node;
        }
    }else if(node->type == kGAI_BehaviorTreeNodeType_Sequence){
        gai_behavior_tree__evaluate_sequence(result, self, node, 0, ud);
        if(result->result==SDK_TRUE){
            return result->node;
        }
    }else if(node->type == kGAI_BehaviorTreeNodeType_Condition){
        /* invalid!!! */
    }
    
    return 0;
}


static gai_behavior_tree_evaluate_result_t* gai_behavior_tree_continue_evaluation(gai_behavior_tree_evaluate_result_t* result
                                                                         , gai_behavior_tree_t* self
                                                                         , gai_behavior_tree_node_t* node, void* ud)
{
    gai_behavior_tree_node_t * parent = node->parent;
    gai_behavior_tree_node_t * child = node;
    sdk_list_node_t * next_child_list_node;
    
    while(parent!=0){
        if(parent->type == kGAI_BehaviorTreeNodeType_Sequence){
            next_child_list_node = SDK_LIST_NEXT(&child->child_node);
            if(next_child_list_node!=&parent->children_list){
                child = SDK_LIST_DATA(next_child_list_node, gai_behavior_tree_node_t, child_node);
                return gai_behavior_tree__evaluate_sequence(result, self, parent, child, 0);
            }
        }
        
        child = parent;
        parent = child->parent;
    }
    result->node = 0;
    result->result = SDK_FALSE;
    return result;
}


gai_err_t gai_behavior_tree_update(gai_behavior_tree_t* tree, void* ud){
    gai_behavior_tree_evaluate_result_t result;
    if(tree->current_node==0){
        tree->current_node = gai_behavior_tree_evaluate_node(&result, tree, tree->root, ud);
    }
    
    if(tree->current_node && tree->current_node->action){
        int status = tree->current_node->action->status;
        if(status==GAI_ACTION_STATUS_UNINITIALIZED){
            gai_action_initialize(tree->current_node->action, ud);
        }else if(status==GAI_ACTION_STATUS_TERMINATED){
            gai_action_cleanup(tree->current_node->action, ud);
            gai_behavior_tree_continue_evaluation(&result, tree, tree->current_node, ud);
            tree->current_node = result.node;
        }else if(status==GAI_ACTION_STATUS_RUNNING){
            gai_action_update(tree->current_node->action, ud);
        }
    }
}