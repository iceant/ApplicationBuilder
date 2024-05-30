#include <sdk_value.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdk_macros.h"

static void sdk_value_char_print(sdk_value_t *value){
    if(value->spec.type.value_type==kSDK_ValueType_Char){
        printf("%c", value->value.char_value);
    }
}

static void sdk_value_integer_print(sdk_value_t * value){
    if(value->spec.type.value_type==kSDK_ValueType_Integer){
        printf("%d", value->value.integer_value);
    }
}

static void sdk_value_double_print(sdk_value_t * value){
    if(value->spec.type.value_type==kSDK_ValueType_Double){
        printf("%f", value->value.double_value);
    }
}

static void sdk_value_bool_print(sdk_value_t * value){
    if(value->spec.type.value_type==kSDK_ValueType_Boolean){
        printf("%s", value->value.bool_value?"T":"F");
    }
}

static void sdk_value_string_print(sdk_value_t * value){
    if(value->spec.type.value_type==kSDK_ValueType_String){
        printf("%s", value->value.string_value);
    }
}

static void sdk_value_array_print(sdk_value_t * value){
    if(value->spec.type.value_type==kSDK_ValueType_Array){
        for(int i=0; i<value->spec.type.array_type.array_size; i++){
            switch (value->spec.type.array_type.item_type) {
                case kSDK_ValueType_Char: {
                    char v = ((char *) value->value.array_value)[i];
                    printf("%c ", v);
                    break;
                }
                case kSDK_ValueType_Integer:{
                    int v = ((int*)value->value.array_value)[i];
                    printf("%d ", v);
                    break;
                }
                case kSDK_ValueType_Double:{
                    double v = ((double *)value->value.array_value)[i];
                    printf("%f ", v);
                    break;
                }
                case kSDK_ValueType_Boolean:{
                    bool v = ((bool *)value->value.array_value)[i];
                    printf("%d ", v);
                    break;
                }
                case kSDK_ValueType_String:{
                    char* v = ((char **)value->value.array_value)[i];
                    printf("%s,", v);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void sdk_value_print(sdk_value_t * value){
    switch (value->spec.type.value_type) {
        case kSDK_ValueType_Char:{
            sdk_value_char_print(value);
            break;
        }
        case kSDK_ValueType_Integer:{
            sdk_value_integer_print(value);
            break;
        }
        case kSDK_ValueType_Double:{
            sdk_value_double_print(value);
            break;
        }
        case kSDK_ValueType_Boolean:{
            sdk_value_bool_print(value);
            break;
        }
        case kSDK_ValueType_String:{
            sdk_value_string_print(value);
            break;
        }
        case kSDK_ValueType_Array:{
            sdk_value_array_print(value);
            break;
        }
        default:{
            printf("Unknown Value Type:%d\n", value->spec.type.value_type);
            break;
        }
    }
}

int main(int argc, char** argv){
    sdk_value_t value;

    sdk_value_char(&value, 'a');
    sdk_value_print(&value);
    printf("\n");

    sdk_value_integer(&value, 12);
    sdk_value_print(&value);
    printf("\n");

    sdk_value_double(&value, 3.1415926f);
    sdk_value_print(&value);
    printf("\n");

    sdk_value_boolean(&value, true);
    sdk_value_print(&value);
    printf("\n");

    sdk_value_boolean(&value, false);
    sdk_value_print(&value);
    printf("\n");


    sdk_value_string(&value, "Hello, String Value!");
    sdk_value_print(&value);
    printf("\n");


    int int_array[]={1,2,3};
    sdk_value_array(&value, int_array, SDK_ARRAY_SIZE(int_array), kSDK_ValueType_Integer);
    sdk_value_print(&value);
    printf("\n");


    char char_array[]={'A','x','$'};
    sdk_value_array(&value, char_array, SDK_ARRAY_SIZE(char_array), kSDK_ValueType_Char);
    sdk_value_print(&value);
    printf("\n");
    return 0;
}
