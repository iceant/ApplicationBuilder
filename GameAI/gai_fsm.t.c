#include <gai_fsm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <synchapi.h>


static int rand_int(int min, int max){
    return rand()%(max-min+1)+min;
}


static int Action_Initialize(gai_action_t* action, void* ud){
    printf("Action %s Initialize\n", action->object.name);
}

static int Action_Update(gai_action_t* action, void* ud){
    int val = rand_int(1, 2);
    printf("Action %s Update, status=%d\n", action->object.name, val);
    return val;
}

static int Action_Cleanup(gai_action_t* action, void* ud){
    printf("Action %s Cleanup\n", action->object.name);
}

static int Random_EvaluatorFunction(gai_evaluator_t* evaluator, void* ud){
    int val = rand_int(0,1);
    printf("Eval %s function: %d\n", evaluator->object.name, val);
    return val;
}

int main(int argc, char** argv){
    srand(time(0));
    gai_fsm_t fsm;
    
    gai_fsm_init(&fsm, "FSM", 0);
    
    gai_action_t idleAction;
    gai_action_init(&idleAction, "IdleAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_action_t dieAction;
    gai_action_init(&dieAction, "DieAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    
    
    gai_fsm_state_t idleState;
    gai_fsm_add_state(&fsm, &idleState, "IdleState", &idleAction, 0);
    
    gai_fsm_state_t dieState;
    gai_fsm_add_state(&fsm, &dieState, "DieState", &dieAction, 0);
    
    gai_evaluator_t isNotAliveEvaluator = GAI_EVALUATOR_INIT("isNotAliveEval", Random_EvaluatorFunction, 0);
    
    gai_fsm_transition_t isNotAliveTransition;
    gai_fsm_add_transition(&fsm, &isNotAliveTransition, "isNotAliveTrans"
                           , "IdleState", "DieState",&isNotAliveEvaluator, 0);
    
    gai_fsm_set_state(&fsm, "IdleState", 0);
    
    while(1){
        gai_fsm_update(&fsm, 0);
        Sleep(1000);
    }
    
    
    return 0;
}