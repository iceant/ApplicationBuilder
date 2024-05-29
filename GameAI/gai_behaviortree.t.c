#include <gai_behaviortree.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <synchapi.h>
////////////////////////////////////////////////////////////////////////////////
////

static int rand_int(int min, int max){
    return rand()%(max-min+1)+min;
}

static int Random_EvaluatorFunction(gai_evaluator_t* evaluator, void* ud){
    int val = rand_int(0,1);
    printf("Eval %s function: %d\n", evaluator->object.name, val);
    return val;
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
////////////////////////////////////////////////////////////////////////////////
////

int main(int argc, char**argv){
    srand(time(0));
    
    gai_behavior_tree_t tree;
    gai_behavior_tree_init(&tree, "BT1", 0);
    
    gai_behavior_tree_node_t root;
    gai_behavior_tree_node_selector(&root, "root", 0);
    
    gai_behavior_tree_node_t alive_sequence;
    gai_behavior_tree_node_sequence(&alive_sequence, "alive_seq", 0);
    
    gai_evaluator_t is_alive_eval= GAI_EVALUATOR_INIT("is_n_alive_eval", Random_EvaluatorFunction, 0);
    gai_behavior_tree_node_t is_alive_condition;
    gai_behavior_tree_node_condition(&is_alive_condition, "is_n_alive_cond", &is_alive_eval, 0);
    
    gai_action_t die_action;
    gai_action_init(&die_action, "die_action", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behavior_tree_node_t die_action_node;
    gai_behavior_tree_node_action(&die_action_node, "die_action_tnode", &die_action, 0);
    
    gai_behavior_tree_node_add_child(&alive_sequence, &is_alive_condition);
    gai_behavior_tree_node_add_child(&alive_sequence, &die_action_node);
    
    gai_behavior_tree_node_add_child(&root, &alive_sequence);
    
    ////////////////////////////////////////////////////////////////////////////////
    ////
    
    gai_behavior_tree_node_t critical_health_sequence;
    gai_behavior_tree_node_sequence(&critical_health_sequence, "critical_health_seq", 0);
    
    gai_evaluator_t has_critical_health_eval= GAI_EVALUATOR_INIT("critical_eval", Random_EvaluatorFunction, 0);
    gai_behavior_tree_node_t has_critical_health_condition;
    gai_behavior_tree_node_condition(&has_critical_health_condition, "has_critical_health", &has_critical_health_eval, 0);
    
    gai_action_t flee_action;
    gai_action_init(&flee_action, "flee_action", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behavior_tree_node_t flee_action_node;
    gai_behavior_tree_node_action(&flee_action_node, "flee_action_tnode", &flee_action, 0);
    
    gai_behavior_tree_node_add_child(&critical_health_sequence, &has_critical_health_condition);
    gai_behavior_tree_node_add_child(&critical_health_sequence, &flee_action_node);
    
    gai_behavior_tree_node_add_child(&root, &critical_health_sequence);
    
    ////////////////////////////////////////////////////////////////////////////////
    ////
    gai_behavior_tree_set_root(&tree, &root);
    while(1){
        gai_behavior_tree_update(&tree, 0);
        Sleep(1000);
    }
    
    return 0;
}