#include <gai_behaviortree.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_behavior_tree_evaluate_result_s{
    gai_behavior_tree_node_t * node;
    sdk_bool_t result;
}gai_behavior_tree_evaluate_result_t;


////////////////////////////////////////////////////////////////////////////////
////

static int gai_behavior_tree__evaluate_sequence(gai_behavior_tree_t* self
                                                , gai_behavior_tree_node_t* node
                                                , void* userdata
                                                , gai_behavior_tree_node_t* current_node_child
                                                , gai_behavior_tree_evaluate_result_t* result);

static int gai_behavior_tree__evaluate_selector(gai_behavior_tree_t* self, gai_behavior_tree_node_t* node, void* userdata, gai_behavior_tree_evaluate_result_t* result){
    sdk_list_node_t *list_node;
    sdk_list_node_t *list_header;
    list_header = &node->children_list;
    int err = 0;
    for(list_node = SDK_LIST_NEXT(list_header); list_node!=list_header; list_node = SDK_LIST_NEXT(list_node)){
        gai_behavior_tree_node_t * child = SDK_LIST_DATA(list_node, gai_behavior_tree_node_t, node);
        switch (child->type) {
            case kGAI_BehaviorTreeNodeType_Action:{
                result->node = child;
                result->result = SDK_TRUE;
                return GAI_OK;
            }
            case kGAI_BehaviorTreeNodeType_Condition:{
                result->result = SDK_FALSE;
                return GAI_OK;
            }
            case kGAI_BehaviorTreeNodeType_Selector:{
                err = gai_behavior_tree__evaluate_selector(self, child, userdata, result);
                if(result->result==SDK_TRUE){
                    /* 评估 sequence时遇到true, 立即返回*/
                    return GAI_OK;
                }
                /* selector 中遇到 false，可以继续评估 */
                break;
            }
            case kGAI_BehaviorTreeNodeType_Sequence:{
                err = gai_behavior_tree__evaluate_sequence(self, child, userdata, 0, result);
                if(result->result){
                    /* 评估 sequence时遇到true, 立即返回*/
                    return GAI_OK;
                }
                break;
            }
        }
    }
    /* 评估完成，没有合适的节点，false */
    result->result = SDK_FALSE;
    return err;
}

static int gai_behavior_tree__evaluate_sequence(gai_behavior_tree_t* self
        , gai_behavior_tree_node_t* node
        , void* userdata
        , gai_behavior_tree_node_t* start_node_child
        , gai_behavior_tree_evaluate_result_t* result)
{
    sdk_list_node_t *list_node;
    sdk_list_node_t *list_header;
    list_header = &node->children_list;
    int err = 0;
    for((list_node = (start_node_child==0)?SDK_LIST_NEXT(list_header):&start_node_child->node)
            ; list_node!=list_header; list_node = SDK_LIST_NEXT(list_node))
    {
        gai_behavior_tree_node_t * child = SDK_LIST_DATA(list_node, gai_behavior_tree_node_t, node);
        switch (child->type) {
            case kGAI_BehaviorTreeNodeType_Action:{
                /* action 默认就是 true */
                result->node = child;
                result->result = SDK_TRUE;
                return GAI_OK;
            }
            case kGAI_BehaviorTreeNodeType_Condition:{
                int eval = gai_evaluator_evaluate(child->evaluator_p);
                if(eval==0){
                    /* sequence 中任何一个节点出现 false，整个 sequence 的结果就是false */
                    result->result = SDK_FALSE;
                    return GAI_OK;
                }
                err = GAI_OK;
                break;
            }
            case kGAI_BehaviorTreeNodeType_Selector:{
                err = gai_behavior_tree__evaluate_selector(self, child, userdata, result);
                if(result->result==SDK_FALSE){
                    /* 结束计算 */
                    return GAI_OK;
                }
                break;
            }
            case kGAI_BehaviorTreeNodeType_Sequence:{
                err = gai_behavior_tree__evaluate_sequence(self, child, userdata, 0, result);
                if(result->result==SDK_FALSE){
                    return GAI_OK;
                }else if(result->result==SDK_TRUE && result->node!=0){
                    return GAI_OK;
                }
                break;
            }
        }
    }
    /* 没有遇到 false 情况 */
    result->result = SDK_TRUE;
    return err;
}

static int gai_behavior_tree__evaluate_node(gai_behavior_tree_t * self, gai_behavior_tree_node_t * node, void* userdata
                                            ,  gai_behavior_tree_node_t ** result){
    switch (node->type) {
        case kGAI_BehaviorTreeNodeType_Action:{
            *result = node;
            return GAI_OK;
        }
        case kGAI_BehaviorTreeNodeType_Condition:{
            /* Never exit!!! */
            return GAI_ERROR;
        }
        case kGAI_BehaviorTreeNodeType_Selector:{
            gai_behavior_tree_evaluate_result_t eval_result={.result=SDK_FALSE, .node = 0};
            int err = gai_behavior_tree__evaluate_selector(self, node, userdata, &eval_result);
            if(eval_result.result){
                *result = eval_result.node;
                return GAI_OK;
            }
        }
        case kGAI_BehaviorTreeNodeType_Sequence:{
            gai_behavior_tree_evaluate_result_t eval_result={.result=SDK_FALSE, .node = 0};
            int err = gai_behavior_tree__evaluate_sequence(self, node, userdata, 0,  &eval_result);
            if(eval_result.result){
                *result = eval_result.node;
                return GAI_OK;
            }
            
        }
    }
    
    return GAI_ERROR;
}

int gai_behavior_tree__continue_evaluation(gai_behavior_tree_t* self, gai_behavior_tree_node_t* node, void* userdata
                                           , gai_behavior_tree_evaluate_result_t * result)
{
    gai_behavior_tree_node_t * parent_node = node->parent_p;
    gai_behavior_tree_node_t * child_node = node;
    
    while(parent_node!=0){
        if(parent_node->type==kGAI_BehaviorTreeNodeType_Sequence){
            sdk_list_node_t * next_child_node = SDK_LIST_NEXT(&child_node->node);
            if(next_child_node!=&parent_node->children_list){
                gai_behavior_tree_node_t * next_child = SDK_LIST_DATA(next_child_node, gai_behavior_tree_node_t, node);
                return gai_behavior_tree__evaluate_sequence(self, parent_node, userdata, next_child, result);
            }
        }
        child_node = parent_node;
        parent_node = child_node->parent_p;
    }
    return GAI_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
////

int gai_behavior_tree_node_init(gai_behavior_tree_node_t * node, gai_behavior_tree_node_type_t type, const char* name)
{
    node->type = type;
    
    size_t name_size = strlen(name);
    memset(node->name, 0, sizeof(node->name));
    name_size = (name_size > (GAI_OBJECT_NAME_SIZE-1))?(GAI_OBJECT_NAME_SIZE-1):name_size;
    memcpy(node->name, name, name_size);
    node->name[name_size]='\0';
    
    SDK_LIST_INIT(&node->children_list);
    SDK_LIST_INIT(&node->node);
    node->action_p =0;
    node->evaluator_p =0;
    node->parent_p =0;
    return GAI_OK;
}

int gai_behavior_tree_node_add_child(gai_behavior_tree_node_t* self, gai_behavior_tree_node_t * child){
    SDK_LIST_INSERT_BEFORE(&self->children_list, &child->node);
    child->parent_p = self;
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_behavior_tree_action_init(gai_behavior_tree_node_t * node, const char* name, gai_action_t* action){
    gai_behavior_tree_node_init(node, kGAI_BehaviorTreeNodeType_Action, name);
    node->action_p = action;
    return GAI_OK;
}

int gai_behavior_tree_condition_init(gai_behavior_tree_node_t * node, const char* name, gai_evaluator_t* evaluator){
    gai_behavior_tree_node_init(node, kGAI_BehaviorTreeNodeType_Condition, name);
    node->evaluator_p = evaluator;
    return GAI_OK;
}

int gai_behavior_tree_selector_init(gai_behavior_tree_node_t * node, const char* name){
    gai_behavior_tree_node_init(node, kGAI_BehaviorTreeNodeType_Selector, name);
    return GAI_OK;
}

int gai_behavior_tree_sequence_init(gai_behavior_tree_node_t * node, const char* name){
    gai_behavior_tree_node_init(node, kGAI_BehaviorTreeNodeType_Sequence, name);
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////




int gai_behavior_tree_init(gai_behavior_tree_t* tree, void* ud){
    tree->current_node = 0;
    tree->root = 0;
    tree->userdata = ud;
    return GAI_OK;
}

int gai_behavior_tree_update(gai_behavior_tree_t* tree, void* userdata)
{
    int err = 0;
    gai_behavior_tree_evaluate_result_t eval_result = {.result = SDK_FALSE, .node = 0};
    if(tree->current_node==0){
        err = gai_behavior_tree__evaluate_node(tree, tree->root, userdata, &tree->current_node);
    }
    
    if(tree->current_node){
        gai_action_status_t status= tree->current_node->action_p->status;
        if(status==kGAI_ActionStatus_UNINITIALIZED){
            gai_action_initialize(tree->current_node->action_p);
        }else if(status==kGAI_ActionStatus_TERMINATED){
            gai_action_cleanup(tree->current_node->action_p);
            gai_behavior_tree__continue_evaluation(tree, tree->current_node, userdata, &eval_result);
            tree->current_node = eval_result.node;
        }else if(status==kGAI_ActionStatus_RUNNING){
            gai_action_update(tree->current_node->action_p);
        }
    }
    
    return GAI_OK;
}

