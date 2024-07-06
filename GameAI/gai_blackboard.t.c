#include <gai_blackboard.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <synchapi.h>


static int rand_in_range(int min, int max){
    return min + rand()%(max-min+1);
}

static int evaluator(gai_knowledge_source_t* source
        , void* userdata
        , gai_knowledge_source_evaluate_result_t* result)
{
    result->confidence = rand_in_range(1, 10);
    result->evaluation = (void*)rand_in_range(1, 100);
    
    printf("exec knowledge source: evaluator: confidence = %d, result=%d\n", result->confidence, (int)result->evaluation);
}

int main(int arg, char** argv){
    srand(time(0));
    
    gai_blackboard_t blackboard;
    gai_blackboard_init(&blackboard, 0);
    
    gai_blackboard_set(&blackboard, "name", "chen peng");
    
    char* name = 0;
    gai_blackboard_get(&blackboard, "name", (void**)&name);
    printf("name: %s\n", name);
    
    gai_knowledge_source_t value_source;
    gai_knowledge_source_init(&value_source, evaluator, 1000);
    gai_blackboard_add_knowledge_source(&blackboard, "value", &value_source);
    
    gai_knowledge_source_t value_source2;
    gai_knowledge_source_init(&value_source2, evaluator, 1000);
    gai_blackboard_add_knowledge_source(&blackboard, "value", &value_source2);
    
    for(int i=0; i<10; i++){
        int* value = 0;
        gai_blackboard_get(&blackboard, "value", (void**)&value);
        printf("[%d] %d\n", i, (int)value);
        Sleep(1000);
    }
    
    gai_blackboard_destroy(&blackboard);
    return 0;
}