#include <sdk_value.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////


void sdk_value_char(sdk_value_t *value, char v)
{
    assert(value);
    value->value.char_value = v;
    value->spec.type.value_type = kSDK_ValueType_Char;

}

void sdk_value_integer(sdk_value_t *value, int v)
{
    assert(value);
    value->value.integer_value = v;
    value->spec.type.value_type = kSDK_ValueType_Integer;
}

void sdk_value_double(sdk_value_t *value, double v)
{
    assert(value);
    value->value.double_value = v;
    value->spec.type.value_type = kSDK_ValueType_Double;
}

void sdk_value_boolean(sdk_value_t *value, bool v)
{
    assert(value);
    value->value.bool_value = v;
    value->spec.type.value_type = kSDK_ValueType_Boolean;
}

void sdk_value_string(sdk_value_t *value, char* v)
{
    assert(value);
    value->value.string_value = v;
    value->spec.type.value_type = kSDK_ValueType_String;
}

void sdk_value_array(sdk_value_t *value, void** array, sdk_size_t size, sdk_value_type_t item_type)
{
    assert(value);

    value->value.array_value = array;
    value->spec.type.array_type.value_type = kSDK_ValueType_Array;
    value->spec.type.array_type.item_type = item_type;
    value->spec.type.array_type.array_size = size;
}

void sdk_value_object(sdk_value_t *value, void* object, sdk_size_t object_size, int object_type)
{
    assert(value);

    value->value.struct_value = object;
    value->spec.type.struct_type.value_type = kSDK_ValueType_Struct;
    value->spec.type.struct_type.object_type = object_type;
    value->spec.type.struct_type.object_size = object_size;
}


