#pragma once

#include <jansson.h>

enum jsondiff_flag {
    JSONDIFF_DEBUG = 1 << 0,
};

json_t *
jsondiff_compare(json_t *a, json_t *b, int flags);

json_t *
jsondiff_apply(json_t *json, json_t *diff, int flags);
