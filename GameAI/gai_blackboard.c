#include <gai_blackboard.h>
#include <sdk_memory.h>
#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////
////

static void* sdk_vector_cp(const void *data, void *arg) {
    return (void*)data;
}

static bool  sdk_vector_eq(const void *data1, const void *data2, void *arg) {
    return (data1==data2)?true:false;
}

static void sdk_vector_free(void *data, void *arg) {
//    printf("free vector: %p\n", data);
    sdk_vector_destroy((sdk_vector_t*)data);
    SDK_FREE(data);
}

sdk_hashtable_val_ops_t sdk_vector_val_ops={.cp=sdk_vector_cp, .free=sdk_vector_free, .eq=sdk_vector_eq, .arg=0};

////////////////////////////////////////////////////////////////////////////////
////


int gai_blackboard_init(gai_blackboard_t* blackboard, void* userdata)
{
    sdk_hashtable_init(&blackboard->attributes, 1, sdk_hashtable_string_key_ops, sdk_hashtable_ptr_val_ops);
    sdk_hashtable_init(&blackboard->sources, 1, sdk_hashtable_string_key_ops, sdk_vector_val_ops);
    
    blackboard->userdata = userdata;
    
    return GAI_OK;
}

void gai_blackboard_destroy(gai_blackboard_t * blackboard)
{
    sdk_hashtable_destroy(&blackboard->attributes);
    sdk_hashtable_destroy(&blackboard->sources);
}

int gai_blackboard_add_knowledge_source(gai_blackboard_t *blackboard, const char* attributeName
                                        , gai_knowledge_source_t * knowledgeSource)
{
    sdk_vector_t * source_list = sdk_hashtable_get(&blackboard->sources, attributeName);
    if(source_list==0){
        SDK_NEW(source_list);
        if(!source_list){
            return GAI_ERROR;
        }
        sdk_vector_init(source_list, 1);
        sdk_hashtable_put(&blackboard->sources, attributeName, source_list);
    }
    
    int err = sdk_vector_resize_add(source_list, knowledgeSource);
    if(err!=SDK_EOK){
        return GAI_ERROR;
    }
    return GAI_OK;
}

int gai_blackboard_remove_knowledge_source(gai_blackboard_t * blackboard, const char* attribute, gai_knowledge_source_t* item)
{
    sdk_vector_t* source_list = sdk_hashtable_get(&blackboard->sources, attribute);
    if(source_list){
        for(sdk_size_t  i=0; i<source_list->size; i++){
            gai_knowledge_source_t * source = sdk_vector_get(source_list, i);
            if(source == item){
                sdk_vector_resize_remove(source_list, i);
                return GAI_OK;
            }
        }
    }
    return GAI_ERROR;
}

static int gai_blackboard_evaluate_source(gai_blackboard_t * self, sdk_vector_t * sources, void** result){
    int bestConfidence = 0;
    void* bestResult = 0;
    gai_knowledge_source_evaluate_result_t eval_result={.confidence = 0, .evaluation = 0};
    for(sdk_size_t i=0; i<sources->size; i++){
        gai_knowledge_source_t * source = sdk_vector_get(sources, i);
        gai_knowledge_source_evaluate(source, self->userdata, &eval_result);
        if(eval_result.confidence > bestConfidence){
            bestConfidence = eval_result.confidence;
            bestResult = eval_result.evaluation;
        }
    }
    if(result){
        *result = bestResult;
    }
    return GAI_OK;
}

int gai_blackboard_get(gai_blackboard_t* blackboard, const char* name, void** result){
    void* d_result = 0;
    int err = 0;
    sdk_vector_t* list = sdk_hashtable_get(&blackboard->sources, name);
    if(list){
        gai_blackboard_evaluate_source(blackboard, list, &d_result);
    }else{
        d_result = sdk_hashtable_get(&blackboard->attributes, name);
    }
    
    if(result){
        *result = d_result;
    }
    
    return GAI_OK;
}

int gai_blackboard_set(gai_blackboard_t * blackboard, const char* name, void* value){
    sdk_hashtable_put(&blackboard->attributes, name, value);
}
