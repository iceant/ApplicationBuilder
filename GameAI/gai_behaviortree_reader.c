#include <gai_behaviortree_reader.h>
#include <stdio.h>
#include <sdk_memory.h>
#include <sdk_str.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
////
static const char* ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static int gai_behaviortree__count_leading_space(const char* line){
    if(line==NULL){
        return 0;
    }
    int result = 0;
    char* s = (char*)line;
    int idx = 0;
    while(s[idx]!='\0'){
        if(s[idx]==' ' && s[idx+1]==' ' && s[idx+2]==' ' && s[idx+3]==' '){
            result+=1;
            idx+=4;
        }else if(s[idx]=='\t'){
            result+=1;
            idx+=1;
        }else{
            idx+=1;
        }
    }
    
    return result;
}

static gai_behavior_tree_node_type_t gai_behaviortree_reader__get_node_type(const char* line){
    int start = sdk_str_upto(line, 1, 0, ALPHABET);
    int end = sdk_str_upto(line, start, 0, ":\n");
    if(sdk_str_cmp(line, start, end, "selector", 1, 0)==0){
        return kGAI_BehaviorTreeNodeType_Selector;
    }else if(sdk_str_cmp(line, start, end, "sequence", 1, 0)==0){
        return kGAI_BehaviorTreeNodeType_Sequence;
    }else if(sdk_str_cmp(line, start, end, "action", 1, 0)==0){
        return kGAI_BehaviorTreeNodeType_Action;
    }else if(sdk_str_cmp(line, start, end, "condition", 1, 0)==0){
        return kGAI_BehaviorTreeNodeType_Condition;
    }
    return -1;
}

static char* gai_behaviortree_reader__get_node_name(const char* line){
    int end = sdk_str_upto(line, 1, 0, ":");
    if(end==0){
        return "";
    }else{
        return sdk_str_trim(sdk_str_sub(line, end+1, 0));
    }
}

////////////////////////////////////////////////////////////////////////////////
////

int gai_behaviortree_read(gai_behavior_tree_t* tree, const char* file, int line_buffer_size){
    FILE* fp = fopen(file, "r");
    if(fp==NULL){
        return GAI_ERROR;
    }
    int level = 0;
    int last_level =0;
    int err = GAI_OK;
    char* line_buffer = SDK_ALLOC(line_buffer_size * sizeof(char));
    gai_behavior_tree_node_t * last_node = 0;
    while((fgets(line_buffer, line_buffer_size, fp))!=NULL){
        level = gai_behaviortree__count_leading_space(line_buffer);
        printf("[%d] %s", level, line_buffer);
        if(last_node==0 && level!=0){
            /* wrong file format */
            err = GAI_ERROR;
            goto __gai_behaviortree_read_exit;
        }
        gai_behavior_tree_node_type_t node_type = gai_behaviortree_reader__get_node_type(line_buffer);
        char* node_name = gai_behaviortree_reader__get_node_name(line_buffer);
        
        
        gai_behavior_tree_node_t* node = SDK_ALLOC(sizeof(*node));
        if(node==NULL){
            err = GAI_ERROR;
            goto __gai_behaviortree_read_exit;
        }
        gai_behavior_tree_node_init(node, node_type, node_name);
        sdk_str_free(node_name);
        if(level==0){
            tree->root = node;
        }else if(level==last_level){
            /* sibling of last node */
            if(last_node){
                if(last_node->parent_p){
                    gai_behavior_tree_node_add_child(last_node->parent_p, node);
                }
            }
        }else if(level > last_level){
            /* child of last node */
            gai_behavior_tree_node_add_child(last_node, node);
        }else if(level < last_level){
            /* sibling of last node's parent */
            gai_behavior_tree_node_add_child(last_node->parent_p->parent_p, node);
        }
        
        last_node = node;
        last_level = level;
    }
__gai_behaviortree_read_exit:
    fclose(fp);
    SDK_FREE(line_buffer);
    return err;
}
