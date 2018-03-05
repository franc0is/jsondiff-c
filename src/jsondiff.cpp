#include <assert.h>
#include <jsondiff.h>
#include <math.h>
#include <set>
#include <stdbool.h>
#include <string>
#include <diff_match_patch.h>

using namespace std;

#define WTF() assert(1);

diff_match_patch<string> dmp;

static json_t *
prv_replace_op(json_t *new_object)
{
    return json_pack("{s:s, s:o}", "o", "r", "v", new_object);
}

static json_t *
prv_delete_op(void)
{
    return json_pack("{s:s}", "o", "-");
}

static json_t *
prv_add_op(json_t *new_object)
{
    return json_pack("{s:s, s:o}", "o", "+", "v", new_object);
}

static json_t *
prv_integer_diff(json_t *a, json_t *b, int flags)
{
    assert(json_is_integer(a) && json_is_integer(b));

    json_int_t delta = json_integer_value(b) - json_integer_value(a);
    return json_pack("{s:s, s:i}", "o", "I", "v", delta);
}

static json_t *
prv_string_diff(json_t *a, json_t *b, int flags)
{
    assert(json_is_string(a) && json_is_string(b));
    string stringa = json_string_value(a);
    string stringb = json_string_value(b);
    auto diffs = dmp.diff_main(stringa, stringb);
    if (diffs.size() > 2) {
        dmp.diff_cleanupEfficiency(diffs);
    }
    if (diffs.size() > 0) {
        string s = dmp.diff_toDelta(diffs);
        return json_pack("{s:s s:s}", "o", "d", "v", s.c_str());
    } else {
        return NULL;
    }
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
        } else if ((diff = jsondiff_diff(value, b_value, flags))) {
            json_object_set(changes, key, diff);
            json_object_del(b_copy, key);
        }
    }

    // add all objects left in B
    json_object_foreach(b_copy, key, value) {
        json_object_set(changes, key, prv_add_op(value));
    }

    return json_pack("{s:s:, s:o}", "o", "O", "v", changes);
}

static int
prv_array_common_prefix(json_t *a, json_t *b)
{
    int minlen = min(json_array_size(a), json_array_size(b));

    for (int i = 0; i < minlen; ++i) {
        json_t *val_a = json_array_get(a, i);
        json_t *val_b = json_array_get(b, i);
        if (!json_equal(val_a, val_b)) {
            return i;
        }
    }

    return minlen;
}

static int
prv_array_common_suffix(json_t *a, json_t *b)
{
    int len_a = json_array_size(a);
    int len_b = json_array_size(b);
    int minlen = min(len_a, len_b);

    for (int i = 0; i < minlen; ++i) {
        json_t *val_a = json_array_get(a, len_a - i - 1);
        json_t *val_b = json_array_get(b, len_b - i - 1);
        if (!json_equal(val_a, val_b)) {
            return i;
        }
    }

    return minlen;
}

static json_t *
prv_array_diff(json_t *a, json_t *b, int flags)
{
    assert(json_is_array(a) && json_is_array(b));

    // Optimization: don't go over common prefix / suffix
    // Note: we could do something much smarter here
    int prefix_len = prv_array_common_prefix(a, b);
    int suffix_len = prv_array_common_suffix(a, b);
    int start = prefix_len;
    int end_a = json_array_size(a) - suffix_len;
    int end_b = json_array_size(b) - suffix_len;
    int end = max(end_a, end_b);

    json_t *changes = json_object();
    char key[11] = {0}; // 2^32 - 1 is 10 digits, plus null terminator
    json_t *val_a = NULL;
    json_t *val_b = NULL;

    for (int i = start; i < end; ++i) {
        snprintf(key, sizeof(key), "%d", i);
        if (i < end_a && i < end_b) {
            val_a = json_array_get(a, i);
            val_b = json_array_get(b, i);
            json_object_set(changes, key, jsondiff_diff(val_a, val_b, flags));
        } else if (i < end_a) {
            json_object_set(changes, key, prv_delete_op());
        } else if (i < end_b) {
            val_b = json_array_get(b, i);
            json_object_set(changes, key, prv_add_op(val_b));
        }
    }

    return json_pack("{s:s, s:o}", "o", "L", "v", changes);
}

static json_t *
prv_array_apply(json_t *a, json_t *diff, int flags)
{
    json_t *c = json_deep_copy(a);

    const char *key;
    json_t *op;
    set<int> to_delete;
    json_object_foreach(diff, key, op) {
        const char *op_type = json_string_value(json_object_get(op, "o"));
        json_int_t index = strtoll(key, NULL, 10);
        if (op_type[0] == '-') {
            to_delete.insert(index);
        } else {
            json_t *old_val = json_array_get(c, index);
            json_t *new_val = jsondiff_apply(old_val, op, flags);
            json_array_set(c, index, new_val);
        }
    }

    for (auto delete_iter = to_delete.rbegin(); delete_iter != to_delete.rend(); ++delete_iter) {
        json_array_remove(c, *delete_iter);
    }

    return c;
}

static json_t *
prv_array_apply_dmp(json_t *a, json_t *diff, int flags)
{
    // TODO
    printf("DMP array apply not implemented\n");
    return NULL;
}

static json_t *
prv_object_apply(json_t *a, json_t *diff, int flags)
{
    json_t *c = json_deep_copy(a);
    const char *key;
    json_t *op;
    json_object_foreach(diff, key, op) {
        // XXX more validation?
        const char *op_type = json_string_value(json_object_get(op, "o"));
        if (op_type[0] == '-') {
            json_object_del(c, key);
        } else {
            json_t *old_val = json_object_get(c, key);
            json_t *new_val = jsondiff_apply(old_val, op, flags);
            json_object_set(c, key, new_val);
        }
    }

    return c;
}

static json_t *
prv_int_apply(json_t *a, json_t *diff, int flags)
{
    assert(json_is_integer(a) && json_is_integer(diff));

    json_int_t a_val = json_integer_value(a);
    json_int_t diff_val = json_integer_value(diff);

    return json_integer(a_val + diff_val);
}

static json_t *
prv_string_apply(json_t *a, json_t *diff, int flags)
{
    assert(json_is_string(a) && json_is_string(diff));

    string a_str = json_string_value(a);
    string diff_str = json_string_value(diff);
    auto dmp_diffs  = dmp.diff_fromDelta(a_str, diff_str);
    auto dmp_patches = dmp.patch_make(a_str, dmp_diffs);
    auto dmp_results = dmp.patch_apply(dmp_patches, a_str);
    return json_string(dmp_results.first.c_str());
}

json_t *
jsondiff_diff(json_t *a, json_t *b, int flags)
{
    if (a == NULL) {
        return NULL;
    }

    if (b == NULL) {
        return prv_delete_op();
    }

    if (json_equal(a, b)) {
        return NULL;
    }

    int a_type = json_typeof(a);
    int b_type = json_typeof(b);

    if (a_type != b_type &&
        !(json_is_boolean(a) && json_is_boolean(b))) {
        // types do not match
        return prv_replace_op(b);
    }

    switch (a_type) {
        case JSON_OBJECT:
            return prv_object_diff(a, b, flags);
        case JSON_ARRAY:
            return prv_array_diff(a, b, flags);
        case JSON_STRING:
            return prv_string_diff(a, b, flags);
        case JSON_INTEGER:
            return prv_integer_diff(a, b, flags);
        case JSON_REAL:
        case JSON_TRUE:
        case JSON_FALSE:
            return prv_replace_op(b);
        case JSON_NULL:
            assert(false); // two NULLs should get caught by the 'json_equal' case
            break;
    }

    return NULL;
}

json_t *
jsondiff_apply(json_t *json, json_t *diff, int flags)
{
    assert(diff && json_is_object(diff));

    const char *op = json_string_value(json_object_get(diff, "o"));
    json_t *diff_data = json_object_get(diff, "v");
    switch (op[0]) {
        case '-':
            return NULL;
        case 'r':
        case '+':
            return diff_data;
        case 'I':
            return prv_int_apply(json, diff_data, flags);
        case 'L':
            return prv_array_apply(json, diff_data, flags);
        case 'O':
            return prv_object_apply(json, diff_data, flags);
        case 'd':
            if (op[1] == '\0') {
                return prv_string_apply(json, diff_data, flags);
            } else if (op[1] == 'L') {
                return prv_array_apply_dmp(json, diff_data, flags);
            }
        default:
            printf("Unrecognized op: %s\n", op);
            return NULL;
    }
}
