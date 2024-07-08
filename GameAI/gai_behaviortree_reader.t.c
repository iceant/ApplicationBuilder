#include "gai_behaviortree_reader.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
////
static char* gai_behaviortree_node_type_name(gai_behavior_tree_node_type_t type){
    switch (type) {
        case kGAI_BehaviorTreeNodeType_Condition:{
            return "Condition";
        }
        case kGAI_BehaviorTreeNodeType_Action:{
            return "Action";
        }
        case kGAI_BehaviorTreeNodeType_Sequence:{
            return "Sequence";
        }
        case kGAI_BehaviorTreeNodeType_Selector:{
            return "Selector";
        }
    }
}

static void show_child(gai_behavior_tree_node_t* parent, int level){
    sdk_list_node_t * node;
    sdk_list_node_t * header = &parent->children_list;
    
    for(int i=0; i<level; i++){
        printf("\t");
    }
    printf("%s[%s]\n", gai_behaviortree_node_type_name(parent->type), parent->name);
    
    for(node= SDK_LIST_NEXT(header); node!=header; node= SDK_LIST_NEXT(node)){
        gai_behavior_tree_node_t* p = SDK_LIST_DATA(node, gai_behavior_tree_node_t, node);
        show_child(p, level+1);
    }
}
////////////////////////////////////////////////////////////////////////////////
////


int main(int argc, char** argv){
    gai_behavior_tree_t tree;
    gai_behaviortree_read(&tree, "M:\\Projects\\GameAI\\GameAI-Core\\src\\main\\resources\\soldier.bt", 1024);
    
    gai_behavior_tree_node_t * root = tree.root;
    printf("\n");
    show_child(root, 0);
    
    return 0;
}