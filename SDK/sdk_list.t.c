#include <sdk_list.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_memory.h>

void sdk_list__print(const char* name, sdk_list_node_t * node){
    printf("[%p] <-prev- [%s:%p] -next-> [%p]\n", node->prev, name, node, node->next);
}

#define PRINTL(N) sdk_list__print(#N, &N)

static void test0(void){
    printf("TEST SDK_LIST_INSERT_BEFORE & SDK_LIST_INSERT_AFTER \n");
    sdk_list_t list_a;
    sdk_list_t list_b;
    sdk_list_t list_c;
    
    sdk_list_node_t list_a_p;
    sdk_list_node_t list_a_n;
    
    sdk_list_node_t list_b_p;
    sdk_list_node_t list_b_n;
    
    sdk_list_node_t list_c_p;
    sdk_list_node_t list_c_n;
    
    SDK_LIST_INIT(&list_a);
    SDK_LIST_INIT(&list_b);
    SDK_LIST_INIT(&list_c);
    SDK_LIST_INIT(&list_a_p);
    SDK_LIST_INIT(&list_a_n);
    SDK_LIST_INIT(&list_b_p);
    SDK_LIST_INIT(&list_b_n);
    SDK_LIST_INIT(&list_c_p);
    SDK_LIST_INIT(&list_c_n);
    
    
    SDK_LIST_INSERT_AFTER(&list_a, &list_a_n);
    SDK_LIST_INSERT_BEFORE(&list_a, &list_a_p);
    
    SDK_LIST_INSERT_AFTER(&list_b, &list_b_n);
    SDK_LIST_INSERT_BEFORE(&list_b, &list_b_p);
    
    SDK_LIST_INSERT_AFTER(&list_c, &list_c_n);
    SDK_LIST_INSERT_BEFORE(&list_c, &list_c_p);
    
    PRINTL(list_a);
    PRINTL(list_b);
    PRINTL(list_c);
    PRINTL(list_a_p);
    PRINTL(list_a_n);
    PRINTL(list_b_p);
    PRINTL(list_b_n);
    PRINTL(list_c_p);
    PRINTL(list_c_n);
}

static void test1(void){
    printf("TEST SDK_LIST_ADD \n");
    sdk_list_t list_a;
    sdk_list_t list_b;
    sdk_list_t list_c;
    
    sdk_list_node_t list_a_p;
    sdk_list_node_t list_a_n;
    
    sdk_list_node_t list_b_p;
    sdk_list_node_t list_b_n;
    
    sdk_list_node_t list_c_p;
    sdk_list_node_t list_c_n;
    
    SDK_LIST_INIT(&list_a);
    SDK_LIST_INIT(&list_b);
    SDK_LIST_INIT(&list_c);
    SDK_LIST_INIT(&list_a_p);
    SDK_LIST_INIT(&list_a_n);
    SDK_LIST_INIT(&list_b_p);
    SDK_LIST_INIT(&list_b_n);
    SDK_LIST_INIT(&list_c_p);
    SDK_LIST_INIT(&list_c_n);
    
    
    SDK_LIST_INSERT_AFTER(&list_a, &list_a_n);
    SDK_LIST_INSERT_BEFORE(&list_a, &list_a_p);
    
    SDK_LIST_INSERT_AFTER(&list_b, &list_b_n);
    SDK_LIST_INSERT_BEFORE(&list_b, &list_b_p);
    
    SDK_LIST_INSERT_AFTER(&list_c, &list_c_n);
    SDK_LIST_INSERT_BEFORE(&list_c, &list_c_p);
    
    SDK_LIST_ADD(&list_a, &list_b);
    
    PRINTL(list_a);
    PRINTL(list_b);
    PRINTL(list_c);
    PRINTL(list_a_p);
    PRINTL(list_a_n);
    PRINTL(list_b_p);
    PRINTL(list_b_n);
    PRINTL(list_c_p);
    PRINTL(list_c_n);
}

static void test2(void){
    printf("TEST SDK_LIST_MOVE \n");
    sdk_list_t list_a;
    sdk_list_t list_b;
    sdk_list_t list_c;
    
    sdk_list_node_t list_a_p;
    sdk_list_node_t list_a_n;
    
    sdk_list_node_t list_b_p;
    sdk_list_node_t list_b_n;
    
    sdk_list_node_t list_c_p;
    sdk_list_node_t list_c_n;
    
    SDK_LIST_INIT(&list_a);
    SDK_LIST_INIT(&list_b);
    SDK_LIST_INIT(&list_c);
    SDK_LIST_INIT(&list_a_p);
    SDK_LIST_INIT(&list_a_n);
    SDK_LIST_INIT(&list_b_p);
    SDK_LIST_INIT(&list_b_n);
    SDK_LIST_INIT(&list_c_p);
    SDK_LIST_INIT(&list_c_n);
    
    
    SDK_LIST_INSERT_AFTER(&list_a, &list_a_n);
    SDK_LIST_INSERT_BEFORE(&list_a, &list_a_p);
    
    SDK_LIST_INSERT_AFTER(&list_b, &list_b_n);
    SDK_LIST_INSERT_BEFORE(&list_b, &list_b_p);
    
    SDK_LIST_INSERT_AFTER(&list_c, &list_c_n);
    SDK_LIST_INSERT_BEFORE(&list_c, &list_c_p);
    
    SDK_LIST_MOVE(&list_a, &list_c);
    
    PRINTL(list_a);
    PRINTL(list_b);
    PRINTL(list_c);
    PRINTL(list_a_p);
    PRINTL(list_a_n);
    PRINTL(list_b_p);
    PRINTL(list_b_n);
    PRINTL(list_c_p);
    PRINTL(list_c_n);
}



static void test3(void){
    printf("TEST SDK_LIST_SPLIT(list_a, list_b, list_c) \n");
    sdk_list_t list_a;
    sdk_list_t list_b;
    sdk_list_t list_c;
    
    sdk_list_node_t list_a_p;
    sdk_list_node_t list_a_n;
    
    sdk_list_node_t list_b_p;
    sdk_list_node_t list_b_n;
    
    sdk_list_node_t list_c_p;
    sdk_list_node_t list_c_n;
    
    SDK_LIST_INIT(&list_a);
    SDK_LIST_INIT(&list_b);
    SDK_LIST_INIT(&list_c);
    SDK_LIST_INIT(&list_a_p);
    SDK_LIST_INIT(&list_a_n);
    SDK_LIST_INIT(&list_b_p);
    SDK_LIST_INIT(&list_b_n);
    SDK_LIST_INIT(&list_c_p);
    SDK_LIST_INIT(&list_c_n);
    
    
    SDK_LIST_INSERT_AFTER(&list_a, &list_a_n);
    SDK_LIST_INSERT_BEFORE(&list_a, &list_a_p);
    
    SDK_LIST_INSERT_AFTER(&list_b, &list_b_n);
    SDK_LIST_INSERT_BEFORE(&list_b, &list_b_p);
    
    SDK_LIST_INSERT_AFTER(&list_c, &list_c_n);
    SDK_LIST_INSERT_BEFORE(&list_c, &list_c_p);
    
//    SDK_LIST_ADD(&list_a, &list_b);
    SDK_LIST_SPLIT(&list_a, &list_a_n, &list_c);
    
    PRINTL(list_a);
    PRINTL(list_b);
    PRINTL(list_c);
    PRINTL(list_a_p);
    PRINTL(list_a_n);
    PRINTL(list_b_p);
    PRINTL(list_b_n);
    PRINTL(list_c_p);
    PRINTL(list_c_n);
}

int main(int argc, char** argv){
    
    test0();
    
    /*
     * list_a->list_a_n->list_a_p->list_a
     * list_b->list_b_n->list_b_p->list_b
     * 执行 SDK_LIST_ADD(list_a, list_b) 之后
     *
     * list_a -> list_a_n -> list_a_p ->list_b_n -> list_b_p -> |
     *   ↑------------------------------------------------------|
     *
     * list_b -> |
     *   ↑ ----- |
     *
     */
    test1();
    
    
    /*
     * list_a->list_a_n->list_a_p->list_a
     * list_b->list_b_n->list_b_p->list_b
     * 执行 SDK_LIST_MOVE(list_a, list_c) 之后
     *
     * list_c -> list_a_n -> list_a_p
     *   ↑------------------------|
     *
     * list_a -> |
     *   ↑ ----- |
     *
     */
    test2();
    
    /*
     * TEST SDK_LIST_SPLIT(list_a, list_a_n, list_c):
     *
     * list_a -> |
     *   ↑ ----- |
     *
     * list_a_n -> list_a_p -> list_c
     *   ↑ ------------------------|
     */
    test3();
    
    return 0;
}