#include <gai_fsm.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
////////////////////////////////////////////////////////////////////////////////
////
static int rand_in_range(int min, int max){
    return min + rand()%(max-min+1);
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
    printf("Action: %s Exit \n", action->object.name);
    exit(0);
    return kGAI_ActionStatus_TERMINATED;
}

static int Random_Evaluator_Function(gai_evaluator_t * self){
    int result =  rand_in_range(0, 1);
    printf("Evaluator:%s -> %d\n", self->object.name, result);
    return result;
}

////////////////////////////////////////////////////////////////////////////////
////
#define FSM_STATE_(N, I, U, C) \
    gai_fsm_state_t N##State; \
    gai_fsm_state_init(&N##State, #N, I, U, C, 0); \
    gai_fsm_add_state(&SoldierFSM, &N##State);
    
#define FSM_STATE(N) FSM_STATE_(N, Action_Initialize, Action_Update, Action_Cleanup)
    
#define FSM_TRANSITION(F, T, E) \
    gai_fsm_transition_t F##_##T##_transition;  \
    gai_fsm_transition_init(&F##_##T##_transition, #T, E, 0); \
    gai_fsm_state_add_transition(&F##State, &F##_##T##_transition)


int main(int argc, char** argv){
    srand(time(0));
    
    gai_fsm_t SoldierFSM;
    gai_fsm_init(&SoldierFSM, "SoldierFSM", 0);
    
    FSM_STATE_(die,Action_Initialize, Action_Update, Action_Exit);
    FSM_STATE(flee);
    FSM_STATE(idle);
    FSM_STATE(move);
    FSM_STATE(shoot);
    FSM_STATE(pursue);
    FSM_STATE(randomMove);
    FSM_STATE(reload);
    
    
    FSM_TRANSITION(idle, die, Random_Evaluator_Function);
    FSM_TRANSITION(idle, flee, Random_Evaluator_Function);
    FSM_TRANSITION(idle, reload, Random_Evaluator_Function);
    FSM_TRANSITION(idle, shoot, Random_Evaluator_Function);
    FSM_TRANSITION(idle, pursue, Random_Evaluator_Function);
    FSM_TRANSITION(idle, randomMove, Random_Evaluator_Function);
    FSM_TRANSITION(idle, idle, Random_Evaluator_Function);
    
    FSM_TRANSITION(move, die, Random_Evaluator_Function);
    FSM_TRANSITION(move, flee, Random_Evaluator_Function);
    FSM_TRANSITION(move, reload, Random_Evaluator_Function);
    FSM_TRANSITION(move, shoot, Random_Evaluator_Function);
    FSM_TRANSITION(move, pursue, Random_Evaluator_Function);
    FSM_TRANSITION(move, move, Random_Evaluator_Function);
    FSM_TRANSITION(move, randomMove, Random_Evaluator_Function);
    FSM_TRANSITION(move, idle, Random_Evaluator_Function);
    
    FSM_TRANSITION(randomMove, die, Random_Evaluator_Function);
    FSM_TRANSITION(randomMove, move, Random_Evaluator_Function);
    
    FSM_TRANSITION(shoot, die, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, flee, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, reload, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, shoot, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, pursue, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, randomMove, Random_Evaluator_Function);
    FSM_TRANSITION(shoot, idle, Random_Evaluator_Function);
    
    FSM_TRANSITION(flee, die, Random_Evaluator_Function);
    FSM_TRANSITION(flee, flee, Random_Evaluator_Function);
    
    FSM_TRANSITION(pursue, die, Random_Evaluator_Function);
    FSM_TRANSITION(pursue, flee, Random_Evaluator_Function);
    FSM_TRANSITION(pursue, shoot, Random_Evaluator_Function);
    FSM_TRANSITION(pursue, idle, Random_Evaluator_Function);
    
    FSM_TRANSITION(reload, die, Random_Evaluator_Function);
    FSM_TRANSITION(reload, shoot, Random_Evaluator_Function);
    FSM_TRANSITION(reload, pursue, Random_Evaluator_Function);
    FSM_TRANSITION(reload, randomMove, Random_Evaluator_Function);
    FSM_TRANSITION(reload, idle, Random_Evaluator_Function);
    
    gai_fsm_set_state_by_name(&SoldierFSM, "idle");
    
    while(1){
//        if(gai_fsm_update(&SoldierFSM)!=GAI_OK){
//            break;
//        }
        gai_fsm_update(&SoldierFSM);
    }
}
