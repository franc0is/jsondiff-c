#include <jsondiff.h>

static json_t *
prv_replace_op(json_t *new)
{
    
}

static json_t *
prv_object_diff(json_t *a, json_t *b, int flags)
{

}

static json_t *
prv_list_diff(json_t *a, json_t *b, int flags)
{

}

json_t *
jsondiff_compare(json_t *a, json_t *b, int flags)
{
    enum json_type a_type = json_typeof(a);
    enum json_type b_type = json_typeof(b);

    if (a_type != b_type) {
        return prv_replace_op(b);
    }

    switch (a_type) {
        case JSON_OBJECT:
            break;
        case JSON_ARRAY:
            break;
        case JSON_STRING
            break;
        case JSON_INTEGER
            break;
        case JSON_REAL
            break;
        case JSON_TRUE
            break;
        case JSON_FALSE
            break;
        case JSON_NULL
            break;
    }

    return NULL;
}

json_t *
jsondiff_apply(json_t *json, json_t *diff, int flags)
{
    return NULL;
}
