#include <gai_evaluator.h>


gai_err_t gai_evaluator_init(gai_evaluator_t * evaluator, const char* name
        , gai_evaluator_function_t function
        , void* userdata)
{
    if(!evaluator){
        return GAI_EINVAL;
    }
    
    gai_object_init(&evaluator->object, name, kGAI_ObjectType_Evaluator, userdata);
    
    evaluator->function = function;
    
    return GAI_OK;
}

int gai_evaluator_evaluate(gai_evaluator_t* evaluator, void* userdata)
{
    if(!evaluator) return GAI_EINVAL;
    if(evaluator->function){
        return evaluator->function(evaluator, userdata);
    }
    return GAI_EVALUATOR_NOFUNCTION;
}

////////////////////////////////////////////////////////////////////////////////
////

static int gai_evaluator__true_function(gai_evaluator_t* evaluator, void* userdata){
    return 1;
}

static int gai_evaluator__false_function(gai_evaluator_t* evaluator, void* userdata){
    return 0;
}


gai_evaluator_t GAI_TRUE_Evaluator=GAI_EVALUATOR_INIT("TRUE_EVALUATOR", gai_evaluator__true_function, 0);
gai_evaluator_t GAI_FALSE_Evaluator=GAI_EVALUATOR_INIT("FALSE_EVALUATOR", gai_evaluator__false_function, 0);

