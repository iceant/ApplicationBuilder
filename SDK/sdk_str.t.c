#include <sdk_str.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    char* str = "Free software is a matter of liberty, not price.\n"
                "To understand the concept, you should think of 'free' \n"
                "as in 'free speech', not as in 'free beer'";

    char* replaced = sdk_str_replace(str, "ree", "AV");

    printf("str address: %p\n", str);
    printf("replaced address: %p\n", replaced);
    printf("replaced(%d): \n%s\n", sdk_str_len(replaced, 1, 0), replaced);
    sdk_str_free(replaced);

    char* mapped_star = sdk_str_map(str, 1, 0, "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    printf("str mapped: %s\n", mapped_star);
    sdk_str_free(mapped_star);

    /*
     1  2  3  4  5  6  7  8  9  10
     ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
      I  n  t  e  r  f  a  c  e
     ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑   ↑  ↑
     -9 -8 -7 -6 -5 -4 -3 -2 -1  0
     */                          

    char sub[5];
    sdk_str_sub_to(sub, sizeof(sub), "Interface", 6, 10);
    printf("sdk_str_sub_to(sub, sizeof(sub), \"Interface\", 6, 10)=%s\n", sub);

    sdk_str_sub_to(sub, sizeof(sub), "Interface", 6, 0);
    printf("sdk_str_sub_to(sub, sizeof(sub), \"Interface\", 6, 0)=%s\n", sub);

    char dup[13];
    sdk_str_dup_to(dup, sizeof(dup), "Interface", 6, 0, 3);
    printf("sdk_str_dup_to(dup, sizeof(dup), \"Interface\", 6, 0, 3)=%s\n", dup);


    char reverse[5];
    sdk_str_reverse_to(reverse, sizeof(reverse), "Interface", 1, 0);
    printf("sdk_str_reverse_to(reverse, sizeof(reverse), \"Interface\", 1, 0)=%s\n", reverse);

    char cat[11];
    sdk_str_cat_to(cat, sizeof(cat), "Interface", 1, 3, "Interface", 6, 0);
    printf("sdk_str_cat_to(cat, sizeof(cat), \"Interface\", 1, 6, \"Interface\", 6, 0)=%s\n", cat);

    char catv[13];
    sdkstr_catv_to(catv, sizeof(catv), "Interface", 1, 3, "Interface", 4, 6, "Interface", 6, 0, NULL);
    printf("sdkstr_catv_to(catv, \"Interface\", 1, 3, \"Interface\", 4, 6, \"Interface\", 6, 0, NULL)=%s\n", catv);

    char map_to[10];
    sdk_str_map_to(map_to, sizeof(map_to), "Interface", 1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz");
    printf("sdk_str_map_to(map_to, sizeof(map_to), \"Interface\", 1, 0, \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\", \"abcdefghijklmnopqrstuvwxyz\")=\n\t%s\n", map_to);

    sdk_str_map_to(map_to, sizeof(map_to), "Interface", 1, 0,  "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    printf("sdk_str_map_to(map_to, sizeof(map_to), \"Interface\", 1, 0,  \"abcdefghijklmnopqrstuvwxyz\", \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\")=\n\t%s\n", map_to);

    return 0;
}