#include <doctest.h>
#include <jsondiff.h>
#include <jansson.h>
#include <string>

#define A_B_APPLY_TEST(_a, _b, _diff, _description)                     \
TEST_CASE(_description) {                                               \
    json_t *a = json_loads(_a, JSON_ENCODE_ANY, NULL);                  \
    REQUIRE(a);                                                         \
    json_t *diff = json_loads(_diff, JSON_ENCODE_ANY, NULL);            \
    REQUIRE(diff);                                                      \
    json_t *expected = json_loads(_b, JSON_ENCODE_ANY, NULL);           \
    REQUIRE(expected);                                                  \
    json_t *b = jsondiff_apply(a, diff, 0);                             \
    std::string s1 = json_dumps(b, JSON_DECODE_ANY);                    \
    std::string s2 = json_dumps(expected, JSON_DECODE_ANY);             \
    REQUIRE(s1 == s2);                                                  \
}

A_B_APPLY_TEST("{\"a\":\"b\"}",
              "{\"a\":\"c\"}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"d\", \"v\": \"-1\\t+c\"}}}",
              "object, change key string");

A_B_APPLY_TEST("{\"a\":\"b\"}",
              "{\"a\":7}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"r\", \"v\": 7}}}",
              "object, change key string to int");

A_B_APPLY_TEST("{\"a\":\"b\"}",
              "{\"a\":\"b\",\"e\":\"d\"}",
              "{\"o\": \"O\", \"v\": {\"e\": {\"o\": \"+\", \"v\": \"d\"}}}",
              "object, add key");

A_B_APPLY_TEST("{\"a\":\"b\",\"e\":\"d\"}",
              "{\"a\":\"b\"}",
              "{\"o\": \"O\", \"v\": {\"e\": {\"o\": \"-\"}}}",
              "object, remove key");

A_B_APPLY_TEST("{\"a\":1}",
              "{\"a\":2}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"I\", \"v\":1}}}",
              "int, increment");

A_B_APPLY_TEST("{\"a\":3}",
              "{\"a\":2}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"I\", \"v\":-1}}}",
              "int, decrement");

A_B_APPLY_TEST("{\"a\":{\"b\":{\"c\":1}}}",
              "{\"a\":{\"b\":{\"c\":2}}}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"O\", \"v\": {\"b\": {\"o\": \"O\", \"v\": {\"c\": {\"o\" : \"I\", \"v\": 1}}}}}}}",
               "int, increment (embedded object)");

A_B_APPLY_TEST("{\"a\":{\"b\":{\"c\":[1]}}}",
              "{\"a\":{\"b\":{\"c\":[2]}}}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"O\", \"v\": {\"b\": {\"o\": \"O\", \"v\": {\"c\": {\"o\" : \"L\", \"v\": {\"0\": {\"o\": \"I\", \"v\": 1}}}}}}}}}",
              "int, increment (embedded list)");

A_B_APPLY_TEST("{\"a\":{\"b\":{\"c\":[{\"d\":1}]}}}",
              "{\"a\":{\"b\":{\"c\":[{\"d\":2}]}}}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"O\", \"v\": {\"b\": {\"o\": \"O\", \"v\": {\"c\": {\"o\" : \"L\", \"v\": {\"0\": {\"o\": \"O\", \"v\": {\"d\": {\"o\": \"I\", \"v\": 1}}}}}}}}}}}",
              "int, increment (embedded list->object item diff)");

A_B_APPLY_TEST("{\"a\":{\"b\":{\"c\":[{\"d\":1}, 1, {\"e\":\"y\"}]}}}",
              "{\"a\":{\"b\":{\"c\":[{\"d\":2}, 2, {\"e\":\"x\"}]}}}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"O\", \"v\": {\"b\": {\"o\": \"O\", \"v\": {\"c\": \
                  {\"o\" : \"L\", \"v\": { \
                      \"0\": {\"o\": \"O\", \"v\": {\"d\": {\"o\": \"I\", \"v\": 1}}}, \
                      \"1\": {\"o\": \"I\", \"v\": 1}, \
                      \"2\": {\"o\": \"O\", \"v\": {\"e\" : {\"o\": \"d\", \"v\": \"-1\\t+x\"}}}} \
                  }}}}}}}",
              "int, increment (embedded list->object mixed item diff)");

A_B_APPLY_TEST("{\"a\":[1,2,3]}",
              "{\"a\":[1,2,3,4]}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\":\"L\", \"v\":{\"3\":{\"o\":\"+\",\"v\":4}}}}}",
              "list, add element");

A_B_APPLY_TEST("{\"a\":[1,2,3,4]}",
              "{\"a\":[1,2,3]}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\":\"L\", \"v\":{\"3\":{\"o\":\"-\"}}}}}",
              "list, substract element");

A_B_APPLY_TEST("{\"a\":[[1],[2,3]]}",
              "{\"a\":[[1],[4]]}",
              "{\"o\": \"O\", \"v\": {\"a\": {\"o\":\"L\", \"v\": {\"1\": {\"o\":\"L\", \"v\":{\"0\":{\"o\":\"I\", \"v\": 2}, \"1\":{\"o\":\"-\"}}}}}}}",
              "nested list");

