#ifndef INCLUDED_GAI_KNOWLEDGESOURCE_H
#define INCLUDED_GAI_KNOWLEDGESOURCE_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/


////////////////////////////////////////////////////////////////////////////////
////
typedef unsigned long gai_time_t;
typedef struct gai_knowledge_source_s gai_knowledge_source_t;

typedef struct gai_knowledge_source_evaluate_result_s{
    int confidence;
    void* evaluation;
}gai_knowledge_source_evaluate_result_t;

typedef int (*gai_knowledge_source_evaluator_t)(gai_knowledge_source_t* source
        , void* userdata
        , gai_knowledge_source_evaluate_result_t* result);

struct gai_knowledge_source_s{
    int confidence;
    void* evaluation;
    gai_knowledge_source_evaluator_t evaluator;
    gai_time_t last_update_time;
    gai_time_t update_frequency;
};


////////////////////////////////////////////////////////////////////////////////
////

int gai_knowledge_source_init(gai_knowledge_source_t* source
                              , gai_knowledge_source_evaluator_t evaluator
                              , gai_time_t update_frequency);

int gai_knowledge_source_evaluate(gai_knowledge_source_t* source, void* userdata
                                  , gai_knowledge_source_evaluate_result_t* result);


#endif /* INCLUDED_GAI_KNOWLEDGESOURCE_H */
