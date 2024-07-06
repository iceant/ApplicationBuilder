#include <gai_behaviortree.h>
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

#define BT_SELECTOR(N) \
    gai_behavior_tree_node_t N##_selector_node; \
    gai_behavior_tree_selector_init(&N##_selector_node, #N)

#define BT_SEQUENCE(N) \
    gai_behavior_tree_node_t N##_sequence_node; \
    gai_behavior_tree_sequence_init(&N##_sequence_node, #N)

#define ACTION(N, I, U, C) \
    gai_behavior_tree_node_t N##_action_node; \
    gai_action_t N##_action; \
    gai_action_init(&N##_action, #N, I, U, C, 0); \
    gai_behavior_tree_action_init(&N##_action_node, #N, &N##_action)

#define ACTION_DEF(N) ACTION(N, Action_Initialize, Action_Update, Action_Cleanup)

#define BT_NODE_CHILD(N, C) \
    gai_behavior_tree_node_add_child(&N, &C)


#define BT_CONDITION(N, F) \
    gai_behavior_tree_node_t N##_condition_node; \
    gai_evaluator_t N##_evaluator; \
    gai_evaluator_init(&N##_evaluator, #N, F, 0); \
    gai_behavior_tree_condition_init(&N##_condition_node, #N, &N##_evaluator)

////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv)
{
    srand(time(0));
    
    gai_behavior_tree_t tree;
    
    gai_behavior_tree_init(&tree, 0);
    
    BT_SELECTOR(root);
    
    BT_SEQUENCE(die);
    BT_SEQUENCE(flee);
    BT_SEQUENCE(has_enemy);
    BT_SEQUENCE(move);
    BT_SEQUENCE(random_move);
    
    ACTION_DEF(idle);
    ACTION(die, Action_Initialize, Action_Update, Action_Exit);
    ACTION_DEF(flee);
    ACTION_DEF(reload);
    ACTION_DEF(shoot);
    ACTION_DEF(pursue);
    ACTION_DEF(move);
    ACTION_DEF(random_move);
    
    
    BT_NODE_CHILD(root_selector_node, die_sequence_node);
    BT_NODE_CHILD(root_selector_node, flee_sequence_node);
    BT_NODE_CHILD(root_selector_node, has_enemy_sequence_node);
    BT_NODE_CHILD(root_selector_node, move_sequence_node);
    BT_NODE_CHILD(root_selector_node, random_move_sequence_node);
    BT_NODE_CHILD(root_selector_node, idle_action_node);
    
    
    BT_CONDITION(is_not_alive, Random_Evaluator_Function);
    BT_NODE_CHILD(die_sequence_node, is_not_alive_condition_node);
    BT_NODE_CHILD(die_sequence_node, die_action_node);
    
    BT_CONDITION(has_critical_health, Random_Evaluator_Function);
    BT_NODE_CHILD(flee_sequence_node, has_critical_health_condition_node);
    BT_NODE_CHILD(flee_sequence_node, flee_action_node);
    
    
    BT_CONDITION(has_enemy, Random_Evaluator_Function);
    BT_NODE_CHILD(has_enemy_sequence_node, has_enemy_condition_node);
    BT_SELECTOR(reload_shoot_pursue);
    BT_NODE_CHILD(has_enemy_sequence_node, reload_shoot_pursue_selector_node);
    
    BT_SEQUENCE(reload);
    BT_NODE_CHILD(reload_shoot_pursue_selector_node, reload_sequence_node);
    BT_CONDITION(has_no_ammo, Random_Evaluator_Function);
    BT_NODE_CHILD(reload_sequence_node, has_no_ammo_condition_node);
    BT_NODE_CHILD(reload_sequence_node, reload_action_node);
    
    BT_SEQUENCE(shoot);
    BT_NODE_CHILD(reload_shoot_pursue_selector_node, shoot_sequence_node);
    BT_CONDITION(can_shoot_enermy, Random_Evaluator_Function);
    BT_NODE_CHILD(shoot_sequence_node, can_shoot_enermy_condition_node);
    BT_NODE_CHILD(shoot_sequence_node, shoot_action_node);
    
    BT_NODE_CHILD(reload_shoot_pursue_selector_node, pursue_action_node);
    
    BT_CONDITION(has_move_position, Random_Evaluator_Function);
    BT_NODE_CHILD(move_sequence_node, has_move_position_condition_node);
    BT_NODE_CHILD(move_sequence_node, move_action_node);
    
    BT_CONDITION(half_chance, Random_Evaluator_Function);
    BT_NODE_CHILD(random_move_sequence_node, half_chance_condition_node);
    BT_NODE_CHILD(random_move_sequence_node, random_move_action_node);
    
    tree.root = &root_selector_node;
    
    while(1){
        gai_behavior_tree_update(&tree, 0);
    }
    
    return 0;
}