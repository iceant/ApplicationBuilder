#include <gai_evaluator.h>

int gai_evaluator_init(gai_evaluator_t* evaluator, const char* name, gai_evaluator_function_t function, void* userdata)
{
    gai_object_init(&evaluator->object, kGAI_ObjectType_Evaluator, name, userdata);
    evaluator->function = function;
    return GAI_OK;
}

int gai_evaluator_evaluate(gai_evaluator_t* evaluator){
    if(!evaluator) return GAI_ERROR;
    return evaluator->function(evaluator);
}
