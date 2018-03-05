#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <jsondiff.h>
#include <jansson.h>

TEST_CASE("object, change key string", "[diff]") {
    json_t *a = json_loads("{\"a\":\"b\"}", JSON_ENCODE_ANY, NULL);
    REQUIRE(a != NULL);
    json_t *b = json_loads("{\"a\":\"c\"}", JSON_ENCODE_ANY, NULL);
    REQUIRE(b != NULL);
    json_t *expected = json_loads("{\"o\": \"O\", \"v\": {\"a\": {\"o\": \"d\", \"v\": \"-1\\t+c\"}}}", JSON_ENCODE_ANY, NULL);
    REQUIRE(expected != NULL);
    json_t *diff = jsondiff_diff(a, b, 0);
    REQUIRE(json_equal(diff, expected));
}

/*
    ["diff", "object, change key string",
        [{"a":"b"}, {"a":"c"}], {"o": "O", "v": {"a": {"o": "d", "v": "-1\t+c"}}}],
    ["diff", "object, change key string to int",
        [{"a":"b"}, {"a":7}], {"o": "O", "v": {"a": {"o": "r", "v": 7}}}],
    ["diff", "object, add key",
        [{"a":"b"}, {"a":"b","e":"d"}], {"o": "O", "v": {"e": {"o": "+", "v": "d"}}}],
    ["diff", "object, remove key",
        [{"a":"b","e":"d"}, {"a":"b"}], {"o": "O", "v": {"e": {"o": "-"}}}],
    ["diff", "int, increment (default diff)",
        [{"a":1}, {"a":2}], {"o": "O", "v": {"a": {"o": "r", "v":2}}}],
    ["diff", "int, increment (increment diff)",
        [{"a":1}, {"a":2}, {"a":{"otype":"integer"}}], {"o": "O", "v": {"a": {"o": "I", "v":1}}}],
    ["diff", "int, decrement (increment diff)",
        [{"a":3}, {"a":2}, {"a":{"otype":"integer"}}], {"o": "O", "v": {"a": {"o": "I", "v":-1}}}],
    ["diff", "int, increment (embedded object diff)",
        [{"a":{"b":{"c":1}}}, {"a":{"b":{"c":2}}},
        {"a":{"attributes":{"b":{"attributes":{"c":{"otype":"integer"}}}}}}],
        {"o": "O", "v": {"a": {"o": "O", "v": {"b": {"o": "O", "v": {"c": {"o" : "I", "v": 1}}}}}}}],
    ["diff", "int, increment (embedded list item diff)",
        [{"a":{"b":{"c":[1]}}}, {"a":{"b":{"c":[2]}}},
        {"a":{"attributes":{"b":{"attributes":{"c":{"otype":"list", "item":{"otype": "integer"}}}}}}}],
        {"o": "O", "v": {"a": {"o": "O", "v": {"b": {"o": "O", "v": {"c": {"o" : "L", "v": {"0": {"o": "I", "v": 1}}}}}}}}}],
    ["diff", "int, increment (embedded list->object item diff)",
        [{"a":{"b":{"c":[{"d":1}]}}}, {"a":{"b":{"c":[{"d":2}]}}},
        {"a":{"attributes":{"b":{"attributes":{"c":{"otype":"list", "item":{"attributes": {"d": {"otype": "integer"}}}}}}}}}],
        {"o": "O", "v": {"a": {"o": "O", "v": {"b": {"o": "O", "v": {"c": {"o" : "L", "v": {"0": {"o": "O", "v": {"d": {"o": "I", "v": 1}}}}}}}}}}}],
    ["diff", "int, increment (embedded list->object mixed item diff)",
        [{"a":{"b":{"c":[{"d":1}, 1, {"e":"y"}]}}}, {"a":{"b":{"c":[{"d":2}, 2, {"e":"x"}]}}},
        {"a":{"attributes":{"b":{"attributes":{"c":
            {
                "otype": "list",
                "item":
                {"attributes": {
                    "d": {"otype": "integer"},
                    "e": {"otype": "replace"}}
                }
            }}}}}}],
        {"o": "O", "v": {"a": {"o": "O", "v": {"b": {"o": "O", "v": {"c":
            {"o" : "L", "v": {
                "0": {"o": "O", "v": {"d": {"o": "I", "v": 1}}},
                "1": {"o": "r", "v": 2},
                "2": {"o": "O", "v": {"e" : {"o": "r", "v": "x"}}}}
            }}}}}}}],
    ["diff", "list, add element",
        [{"a":[1,2,3]}, {"a":[1,2,3,4]}, {"a":{"otype":"list"}}], {"o": "O", "v": {"a": {"o":"L", "v":{"3":{"o":"+","v":4}}}}}],
    ["diff", "list, subtract element",
        [{"a":[1,2,3,4]}, {"a":[1,2,3]}, {"a":{"otype":"list"}}], {"o": "O", "v": {"a": {"o":"L", "v":{"3":{"o":"-"}}}}}],
    ["diff", "list dmp, add element",
        [{"a":[1,2,3]}, {"a":[1,2,3,4]}, {"a":{"otype":"list_dmp"}}], {"o": "O", "v": {"a": {"o":"dL", "v":"=6\t+4%0A"}}}],
    ["diff", "list dmp, subtract element",
        [{"a":[1,2,3,4]}, {"a":[1,2,3]}, {"a":{"otype":"list_dmp"}}], {"o": "O", "v": {"a": {"o":"dL", "v":"=6\t-2"}}}],
    ["diff", "list dmp, clear elements",
        [{"a":[1,2,3,4]}, {"a":[]}, {"a":{"otype":"list_dmp"}}], {"o": "O", "v": {"a": {"o":"dL", "v":"-8"}}}],
    ["diff", "list dmp, add elements",
        [{"a":[]}, {"a":[1,2]}, {"a":{"otype":"list_dmp"}}], {"o": "O", "v": {"a": {"o":"dL", "v":"+1%0A2%0A"}}}],
    ["diff", "nested list, subtract element",
        [{"a":[[1],[2,3]]}, {"a":[[1],[4]]}, {"a":{"otype":"list", "item":{"otype":"list", "item":{"otype": "integer"}}}}], {"o": "O", "v": {"a": {"o":"L", "v": {"1": {"o":"L", "v":{"0":{"o":"I", "v": 2}, "1":{"o":"-"}}}}}}}],
    ["transform", "list, transform",
        [{"a":[1,2,3]}, {"a":[1,2,3,5]}, {"a":[1,2,3,4]}, {"a":{"otype":"list"}}], {"a":[1,2,3,5,4]}],
    ["transform", "list (replace), transform2",
        [{"a":[1,2,3]}, {"a":[1,2]}, {"a":[1,2,4]}], {"a":[1,2]}],
    ["transform", "list, transform2",
        [{"a":[1,2,3]}, {"a":[1,2]}, {"a":[1,2,4]}, {"a":{"otype":"list"}}], {"a":[1,2]}],
    ["transform", "list, transform3",
        [{"a":[1,2,3]}, {"a":[1,2,4]}, {"a":[1,2]}, {"a":{"otype":"list"}}], {"a":[1,2,4]}],
    ["transform", "list, transform4",
        [{"a":[1,2,3]}, {"a":[1,2,3,4]}, {"a":[1,2,3,5]}, {"a":{"otype":"list"}}], {"a":[1,2,3,4,5]}],
    ["transform", "list, transform4 (dmp)",
        [{"a":[1,2,3]}, {"a":[1,2,3,4]}, {"a":[1,2,3,5]}, {"a":{"otype":"list_dmp"}}], {"a":[1,2,3,5,4]}],
    ["transform", "list, transform5",
        [{"a":[1,2,3,4]}, {"a":[1,5,2,3,4]}, {"a":[1,6,7,2,3,4]}, {"a":{"otype":"list"}}], {"a":[1,5,6,7,2,3,4]}],
    ["transform", "list, transform5 (dmp)",
        [{"a":[1,2,3,4]}, {"a":[1,5,2,3,4]}, {"a":[1,6,7,2,3,4]}, {"a":{"otype":"list_dmp"}}], {"a":[1,5,6,7,2,3,4]}],
    ["transform", "list, transform6",
        [{"a":[1,2,3,4]}, {"a":[1,6,7,2,3,4]}, {"a":[1,5,2,3,4]}, {"a":{"otype":"list"}}], {"a":[1,6,7,5,2,3,4]}],
    ["transform", "list, transform6 (dmp)",
        [{"a":[1,2,3,4]}, {"a":[1,6,7,2,3,4]}, {"a":[1,5,2,3,4]}, {"a":{"otype":"list_dmp"}}], {"a":[1,5,6,7,2,3,4]}],
    ["apply_offsets", "regular dmp diff, applied with patch_apply_with_offsets",
        [{"a": "this is some text"}, {"a": "this is another text"}], {"a": "this is another text"}]
*/
