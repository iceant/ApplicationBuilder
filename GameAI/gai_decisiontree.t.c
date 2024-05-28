#include <gai_decisiontree.h>
#include <stdio.h>
#include <stdlib.h>
#include <synchapi.h>

////////////////////////////////////////////////////////////////////////////////
////
static int IsNotAliveEvalFunction(gai_evaluator_t * evaluator, void* ud){
    printf("IsNotAliveEvalFunction\n");
    return 1;
}

static int dieAction_Initialize(gai_action_t* action, void* ud){
    printf("dieAction_Initialize\n");
    return 0;
}

static int dieAction_Update(gai_action_t* action, void* ud){
    printf("dieAction_Update\n");
    return GAI_ACTION_STATUS_TERMINATED;
}

static int dieAction_Cleanup(gai_action_t* action, void* ud){
    printf("dieAction_Cleanup\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv)
{
    gai_decision_tree_t tree;
    gai_decision_tree_init(&tree, "DTree1", 0);
    
    gai_decision_tree_node_t isAliveBranch;
    gai_decision_tree_node_init(&isAliveBranch, "isAliveBranch", kGAI_DecisionTreeNodeType_Branch, 0);
    gai_evaluator_t isNotAliveEvaluator = GAI_EVALUATOR_INIT("IsNotAliveEval", IsNotAliveEvalFunction, 0);
    gai_decision_tree_node_set_evaluator(&isAliveBranch, &isNotAliveEvaluator);
    
    gai_decision_tree_node_t criticalBranch;
    gai_decision_tree_node_init(&criticalBranch, "criticalBranch", kGAI_DecisionTreeNodeType_Branch, 0);
    
    gai_decision_tree_node_t dieActionNode;
    gai_action_t dieAction;
    gai_action_init(&dieAction, "dieAction", dieAction_Initialize, dieAction_Update, dieAction_Cleanup, 0);
    gai_decision_tree_node_init(&dieActionNode, "dieActionNode", kGAI_DecisionTreeNodeType_Action, 0);
    dieActionNode.action = &dieAction;
    
    gai_decision_tree_node_add_child(&isAliveBranch, &criticalBranch);
    gai_decision_tree_node_add_child(&isAliveBranch, &dieActionNode);
    
    gai_decision_tree_set_branch(&tree,&isAliveBranch);
    
    while(1){
        gai_decision_tree_update(&tree, 0);
        Sleep(1000);
    }
    
    
    gai_decision_tree_node_destroy(&isAliveBranch);
    gai_decision_tree_node_destroy(&criticalBranch);
    gai_decision_tree_node_destroy(&dieActionNode);
    
    
    return 0;
}