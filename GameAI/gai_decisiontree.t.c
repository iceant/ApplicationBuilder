#include "gai_decisiontree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
////////////////////////////////////////////////////////////////////////////////
////

static int rand_in_range(int min, int max){
    return min + rand()%(max-min+1);
}

static int Random_Evaluator_Function(gai_evaluator_t * self){
    int result =  rand_in_range(0, 1);
    printf("Evaluator:%s -> %d\n", self->object.name, result);
    return result;
}

static int Soldier_Evaluator_IsAlive(gai_evaluator_t * self){
    return rand_in_range(0, 1);
}

static int Soldier_HasCriticalHealth(gai_evaluator_t* self){
    return rand_in_range(0, 1);
}

static int Soldier_HasMovePosition(gai_evaluator_t* self){
    return rand_in_range(0, 1);
}


static int Soldier_HasEnemy(gai_evaluator_t* self){
    return rand_in_range(0, 1);
}

static gai_action_status_t Action_Initialize(gai_action_t* action){
    int result =  rand_in_range(0, 2);
    printf("Action: %s Initialize -> %d\n", action->object.name, result);
    return result;
}

static gai_action_status_t Action_Update(gai_action_t* action){
    int result =  rand_in_range(1, 2);
    printf("Action: %s Update -> %d\n", action->object.name, result);
    return result;
}

static gai_action_status_t Action_Cleanup(gai_action_t* action){
    printf("Action: %s Cleanup \n", action->object.name);
    return kGAI_ActionStatus_TERMINATED;
}

static gai_action_status_t Action_Exit(gai_action_t* action){
    printf("Action Exit: %s \n", action->object.name);
    exit(0);
    return kGAI_ActionStatus_TERMINATED;
}
////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv){
    srand(time(0));
    
    gai_decisiontree_t SoldierDecisionTree;
    
    gai_decisionbranch_t isAliveBranch;
    gai_decisionbranch_t criticalBranch;
    gai_decisionbranch_t moveFleeBranch;
    gai_decisionbranch_t enemyBranch;
    gai_decisionbranch_t ammoBranch;
    gai_decisionbranch_t shootBranch;
    gai_decisionbranch_t moveRandomBranch;
    gai_decisionbranch_t randomBranch;
    
    
    gai_action_t dieAction;
    gai_action_init(&dieAction, "DieAction", Action_Initialize, Action_Update, Action_Exit ,0);
    gai_action_t moveToPositionAction;
    gai_action_init(&moveToPositionAction, "moveToPositionAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t fleeAction;
    gai_action_init(&fleeAction, "fleeAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t reloadAction;
    gai_action_init(&reloadAction, "reloadAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t shootAction;
    gai_action_init(&shootAction, "shootAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t pursueAction;
    gai_action_init(&pursueAction, "pursueAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t randomMoveAction;
    gai_action_init(&randomMoveAction, "randomMoveAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    gai_action_t idleAction;
    gai_action_init(&idleAction, "idleAction", Action_Initialize, Action_Update, Action_Cleanup ,0);
    
    
    gai_decisionbranch_init(&isAliveBranch, "isAliveBranch", 0);
    gai_decisionbranch_init(&criticalBranch, "criticalBranch", 0);
    gai_decisionbranch_init(&moveFleeBranch, "moveFleeBranch", 0);
    gai_decisionbranch_init(&enemyBranch, "enemyBranch", 0);
    gai_decisionbranch_init(&ammoBranch, "ammoBranch", 0);
    gai_decisionbranch_init(&shootBranch, "shootBranch", 0);
    gai_decisionbranch_init(&moveRandomBranch, "moveRandomBranch", 0);
    gai_decisionbranch_init(&randomBranch, "randomBranch", 0);
    
    gai_decisionbranch_set_evaluator(&isAliveBranch, "isAliveBranchEvaluator", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&criticalBranch, "criticalBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&moveFleeBranch, "moveFleeBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&enemyBranch, "enemyBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&ammoBranch, "ammoBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&shootBranch, "shootBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&moveRandomBranch, "moveRandomBranch", Random_Evaluator_Function, 0);
    gai_decisionbranch_set_evaluator(&randomBranch, "randomBranch", Random_Evaluator_Function, 0);

    
    gai_decisionbranch_add_child(&isAliveBranch, (gai_object_t *) &criticalBranch);
    gai_decisionbranch_add_child(&isAliveBranch, (gai_object_t *) &dieAction);
    
    gai_decisionbranch_add_child(&criticalBranch, (gai_object_t *) &moveFleeBranch);
    gai_decisionbranch_add_child(&criticalBranch, (gai_object_t *) &enemyBranch);
    
    /* branch: has move position */

    gai_decisionbranch_add_child(&moveFleeBranch, (gai_object_t *) &moveToPositionAction);
    gai_decisionbranch_add_child(&moveFleeBranch, (gai_object_t *) &fleeAction);
    
    
    gai_decisionbranch_add_child(&enemyBranch, (gai_object_t *) &ammoBranch);
    gai_decisionbranch_add_child(&enemyBranch, (gai_object_t *) &moveRandomBranch);
    
    gai_decisionbranch_add_child(&ammoBranch, (gai_object_t *) &shootBranch);
    gai_decisionbranch_add_child(&ammoBranch, (gai_object_t *) &reloadAction);
    
    
    gai_decisionbranch_add_child(&shootBranch, (gai_object_t *) &shootAction);
    gai_decisionbranch_add_child(&shootBranch, (gai_object_t *) &pursueAction);
    
    gai_decisionbranch_add_child(&moveRandomBranch, (gai_object_t *) &moveToPositionAction);
    gai_decisionbranch_add_child(&moveRandomBranch, (gai_object_t *) &randomBranch);
    
    gai_decisionbranch_add_child(&randomBranch, (gai_object_t *) &randomMoveAction);
    gai_decisionbranch_add_child(&randomBranch, (gai_object_t *) &idleAction);
    
    
    gai_decisiontree_init(&SoldierDecisionTree, "SoldierDecisionTree", 0);
    gai_decisiontree_set_branch(&SoldierDecisionTree, &isAliveBranch);
    
    while(1){
        gai_decisiontree_update(&SoldierDecisionTree, 0);
    }
    
    return 0;
}
