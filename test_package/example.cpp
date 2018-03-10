#include <iostream>
#include "jsondiff.h"

int main() {
    json_t *a = json_loads("{\"a\": 1}", JSON_ENCODE_ANY, NULL);
    json_t *b = json_loads("{\"a\": 2}", JSON_ENCODE_ANY, NULL);
    json_t *diff = jsondiff_diff(a, b, 0);
    printf("%s\n", json_dumps(diff, JSON_DECODE_ANY));
}
