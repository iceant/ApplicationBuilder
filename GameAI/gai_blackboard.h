#ifndef INCLUDED_GAI_BLACKBOARD_H
#define INCLUDED_GAI_BLACKBOARD_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_HASHTABLE_H
#include <sdk_hashtable.h>
#endif /*INCLUDED_SDK_HASHTABLE_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/


#ifndef INCLUDED_GAI_OBJECT_H
#include <gai_object.h>
#endif /*INCLUDED_GAI_OBJECT_H*/


#ifndef INCLUDED_GAI_KNOWLEDGESOURCE_H
#include <gai_knowledgesource.h>
#endif /*INCLUDED_GAI_KNOWLEDGESOURCE_H*/


////////////////////////////////////////////////////////////////////////////////
////

typedef struct gai_blackboard_s{
    sdk_hashtable_t attributes;
    sdk_hashtable_t sources;
    void* userdata;
}gai_blackboard_t;

////////////////////////////////////////////////////////////////////////////////
////

int gai_blackboard_init(gai_blackboard_t* blackboard, void* userdata);

void gai_blackboard_destroy(gai_blackboard_t * blackboard);

int gai_blackboard_add_knowledge_source(gai_blackboard_t *blackboard
                                        , const char* attributeName
                                        , gai_knowledge_source_t * knowledgeSource);

int gai_blackboard_get(gai_blackboard_t* blackboard, const char* name, void** result);

int gai_blackboard_set(gai_blackboard_t * blackboard, const char* name, void* value);

#endif /* INCLUDED_GAI_BLACKBOARD_H */
