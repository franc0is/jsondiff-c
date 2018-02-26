#include <assert.h>
#include <jsondiff.h>
#include <stdbool.h>

static json_t *
prv_replace_op(json_t *new)
{
    return NULL;
}

static json_t *
prv_integer_diff(json_t *a, json_t *b, int flags)
{
    return NULL;
}

static json_t *
prv_string_diff(json_t *a, json_t *b, int flags)
{
    return NULL;
}

static json_t *
prv_object_diff(json_t *a, json_t *b, int flags)
{
    return NULL;
}

static json_t *
prv_array_diff(json_t *a, json_t *b, int flags)
{
    return NULL;
}

json_t *
jsondiff_compare(json_t *a, json_t *b, int flags)
{
    int a_type = json_typeof(a);
    int b_type = json_typeof(b);

    if (json_equal(a, b)) {
        return json_object(); // empty object, XXX should it be null?
    }

    if (a_type != b_type &&
        !(json_is_boolean(a) && json_is_boolean(b))) {
        // types do not match
        return prv_replace_op(b);
    }

    switch (a_type) {
        case JSON_OBJECT:
            return prv_object_diff(a, b, flags);
            break;
        case JSON_ARRAY:
            return prv_array_diff(a, b, flags);
            break;
        case JSON_STRING:
            return prv_string_diff(a, b, flags);
            break;
        case JSON_INTEGER:
            return prv_integer_diff(a, b, flags);
            break;
        case JSON_REAL:
        case JSON_TRUE:
        case JSON_FALSE:
            return prv_replace_op(b);
            break;
        case JSON_NULL:
            assert(false); // two NULLs should get caught by the 'json_equal' case
            break;
    }

    return NULL;
}

json_t *
jsondiff_apply(json_t *json, json_t *diff, int flags)
{
    return NULL;
}
