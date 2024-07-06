#include <gai_knowledgesource.h>
#include <sdk_time.h>

int gai_knowledge_source_init(gai_knowledge_source_t* source
        , gai_knowledge_source_evaluator_t evaluator
        , gai_time_t update_frequency)
{
    source->confidence = 0;
    source->evaluation = 0;
    source->evaluator = evaluator;
    source->last_update_time = 0;
    source->update_frequency = update_frequency;
    
    return GAI_OK;
}


int gai_knowledge_source_evaluate(gai_knowledge_source_t* source, void* userdata, gai_knowledge_source_evaluate_result_t* result)
{
    gai_time_t time = sdk_time_get_current_ms();
    gai_time_t next_update_time = time + source->update_frequency;
    if(next_update_time > source->last_update_time){
        source->last_update_time = time;
        
        gai_knowledge_source_evaluate_result_t eval_result={.confidence = 0, .evaluation = 0};
        source->evaluator(source, userdata, &eval_result);
        source->confidence = eval_result.confidence;
        source->evaluation = eval_result.evaluation;
        
    }
    
    result->confidence = source->confidence;
    result->evaluation = source->evaluation;
    return GAI_OK;
}