#include <gai_decisiontree.h>

int gai_decisiontree_init(gai_decisiontree_t* tree, const char* name, void* userdata)
{
    gai_object_init(&tree->object, kGAI_ObjectType_DecisionTree, name, userdata);
    tree->branch = 0;
    tree->current_action = 0;
    return GAI_OK;
}

int gai_decisiontree_set_branch(gai_decisiontree_t * tree, gai_decisionbranch_t * branch)
{
    tree->branch = branch;
    return GAI_OK;
}

int gai_decisiontree_update(gai_decisiontree_t* tree, void* userdata)
{
    if(!tree) return GAI_EINVAL;
    
    int err = GAI_OK;
    if(tree->branch==0){
        return GAI_OK;
    }
    
    /* Search the tree for an Action to run if not currently executing an Action */
    if(tree->current_action==0){
        err = gai_decisionbranch_evaluate(tree->branch, &tree->current_action);
        if(err!=GAI_OK){
            return err;
        }
        if(tree->current_action){
            err = gai_action_initialize(tree->current_action);
            if(err!=GAI_OK){
                return err;
            }
        }
    }
    
    gai_action_status_t status = gai_action_update(tree->current_action);
    if(status==kGAI_ActionStatus_TERMINATED){
        gai_action_cleanup(tree->current_action);
        tree->current_action = 0;
    }
    
    return GAI_OK;
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_decisionbranch_init(gai_decisionbranch_t* branch, const char* name, void* userdata)
{
    gai_object_init(&branch->object, kGAI_ObjectType_DecisionBranch, name, userdata);
    sdk_vector_init(&branch->children, 0);
    return GAI_OK;
}

void gai_decisionbranch_destroy(gai_decisionbranch_t * branch)
{
    sdk_vector_destroy(&(branch->children));
}

int gai_decisionbranch_set_evaluator(gai_decisionbranch_t * branch, const char* name, gai_evaluator_function_t function, void* userdata)
{
    gai_evaluator_init(&branch->evaluator, name, function, userdata);
    return GAI_OK;
}

int gai_decisionbranch_add_child(gai_decisionbranch_t* branch, gai_object_t * child){
    sdk_vector_t* children = &branch->children;
    int err = sdk_vector_resize_add(children, child);
    if(err!=SDK_EOK){
        return GAI_ERROR;
    }
    
    return GAI_OK;
}

int gai_decisionbranch_evaluate(gai_decisionbranch_t* branch, gai_action_t** result)
{
    int eval = gai_evaluator_evaluate(&branch->evaluator);
    if(eval==GAI_ERROR){
        return eval;
    }
    
    gai_object_t * choice = (gai_object_t*)sdk_vector_get(&branch->children, eval);
    
    if(choice==0){
        return GAI_ERROR;
    }
    
    if(choice->type==kGAI_ObjectType_DecisionBranch){
        return gai_decisionbranch_evaluate((gai_decisionbranch_t*)choice, result);
    }else if(choice->type==kGAI_ObjectType_Action){
        if(result){
           *result = (gai_action_t*)choice;
        }
        return GAI_OK;
    }
    
    return GAI_ERROR;
}
