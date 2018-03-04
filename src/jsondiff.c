#include <assert.h>
#include <jsondiff.h>
#include <stdbool.h>

static json_t *
prv_replace_op(json_t *new)
{
    return json_pack("{s:s, s:O}", "o", "r", "v", new);
}

static json_t *
prv_delete_op(void)
{
    return json_pack("{s:s}", "o", "-");
}

static json_t *
prv_add_op(json_t *new)
{
    return json_pack("{s:s, s:O}", "o", "+", "v", new);
}

static json_t *
prv_modify_op(json_t *changes)
{
    return json_pack("{s:s, s:O}", "o", "M", "v", changes);
}

static json_t *
prv_integer_diff(json_t *a, json_t *b, int flags)
{
    assert(json_is_integer(a) && json_is_integer(b));

    int delta = json_integer_value(b) - json_integer_value(a);
    return json_pack("{s:s, s:i}", "o", "I", "v", delta);
}

static json_t *
prv_string_diff(json_t *a, json_t *b, int flags)
{
    // FIXME
    return prv_replace_op(b);
}

static json_t *
prv_object_diff(json_t *a, json_t *b, int flags)
{
    assert(json_is_object(a) && json_is_object(b));

    // copy b so we can edit it in place
    json_t *b_copy = json_copy(b);

    // remove or modify all objects in A but different in B
    json_t *changes = json_object();
    const char *key;
    json_t *value;
    json_t *diff = NULL;
    json_object_foreach(a, key, value) {
        json_t *b_value = json_object_get(b, key);
        if (!b_value) {
            json_object_set(changes, key, prv_delete_op());
        } else if ((diff = jsondiff_compare(value, b_value, flags))) {
            json_object_set(changes, key, diff);
            json_object_del(b_copy, key);
        }
    }

    // add all objects left in B
    json_object_foreach(b_copy, key, value) {
        json_object_set(changes, key, prv_add_op(value));
    }

    return prv_modify_op(changes);
}

static json_t *
prv_array_diff(json_t *a, json_t *b, int flags)
{
    // FIXME
    return prv_replace_op(b);
}

json_t *
jsondiff_compare(json_t *a, json_t *b, int flags)
{
    int a_type = json_typeof(a);
    int b_type = json_typeof(b);

    if (json_equal(a, b)) {
        return NULL;
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
