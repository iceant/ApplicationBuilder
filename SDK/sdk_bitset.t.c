#include <sdk_bitset.h>
#include <stdlib.h>
#include <stdio.h>

void bitset_print(int n, int bit, void* cl)
{
    printf("%d", bit);
}

static void bitset_show(const char* name, sdk_bitset_t * set){
    printf("== %s ==\n", name);
    sdk_bitset_map(set, bitset_print, 0);
    printf("\n");
}
#define BISET_SHOW(s) bitset_show(#s, s)

int main(int argc, char** argv)
{
    sdk_bitset_t* set1 = sdk_bitset_new(10);

    sdk_bitset_put(set1, 1, 1);
    sdk_bitset_put(set1, 3, 1);
    sdk_bitset_put(set1, 5, 1);

    BISET_SHOW(set1);

    sdk_bitset_t* set2 = sdk_bitset_new(10);
    sdk_bitset_put(set2, 3, 1);
    sdk_bitset_put(set2, 5, 1);
    sdk_bitset_put(set2, 8, 1);

    BISET_SHOW(set2);

    sdk_bitset_t* union_set = sdk_bitset_union(set1, set2);
    BISET_SHOW(union_set);

    sdk_bitset_t* inner_set = sdk_bitset_inter(set1, set2);
    BISET_SHOW(inner_set);

    sdk_bitset_t * diff_set = sdk_bitset_diff(set1, set2);
    BISET_SHOW(diff_set);

    sdk_bitset_t * minus_set = sdk_bitset_minus(set1, set2);
    BISET_SHOW(minus_set);

    BISET_SHOW(set1);
    BISET_SHOW(set2);
    printf("set1 < set2? %d\n", sdk_bitset_lt(set1, set2));
    printf("set2 < set1? %d\n", sdk_bitset_lt(set2, set1));

    printf("set2 == set1? %d\n", sdk_bitset_eq(set2, set1));
    printf("set1 <= set2? %d\n", sdk_bitset_leq(set1, set2));
    printf("set2 <= set1? %d\n", sdk_bitset_leq(set2, set1));
    printf("set2 == set2? %d\n", sdk_bitset_eq(set2, set2));

    sdk_bitset_free(&minus_set);
    sdk_bitset_free(&diff_set);
    sdk_bitset_free(&inner_set);
    sdk_bitset_free(&union_set);
    sdk_bitset_free(&set2);
    sdk_bitset_free(&set1);

}

