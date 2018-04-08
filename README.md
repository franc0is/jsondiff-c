# jsondiff-c
An MIT-licensed C implementation of Simperium's [jsondiff](https://github.com/simperium/jsondiff) library

[![Build Status](https://travis-ci.org/franc0is/jsondiff-c.svg?branch=master)](https://travis-ci.org/franc0is/jsondiff-c)
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/github/franc0is/jsondiff-c?svg=true)](https://ci.appveyor.com/project/franc0is/jsondiff-c)

## Example

```
#include <stdio.h>
#include "jsondiff.h"

int main() {
    json_t *a = json_loads("{\"a\": 1}", JSON_ENCODE_ANY, NULL);
    json_t *b = json_loads("{\"a\": 2}", JSON_ENCODE_ANY, NULL);
    json_t *diff = jsondiff_diff(a, b, 0);
    printf("%s\n", json_dumps(diff, JSON_DECODE_ANY));
}
```

prints: 

```
{
    "o": "O",
    "v": {
        "a": {
            "o": "I",
            "v": 1
        }
    }
}
```

## Status

### What's there

* diff/apply APIs
* a tool (jsondiff-tool) to diff / patch json strings on the command line
* unit tests

### What's TODO

* jsondiff-c on Unix currently requires gcc/g++ 7. I'll fix this at a later date.
* transform APIs (to apply a diff object to another diff object)
* more example programs
* continuous build/test with ~Travis~ (done) & Appveyor
* dependencies are build from source, need to deploy more packages to bintray

## Building

jsondiff-c uses the cmake build system and the conan package management system.
To build it, do the following:

```
# create build folder
$ mkdir build && cd build
# install dependencies via conan - this requires internet access
$ conan install .. -b
# generate build files
$ conan build ..
```

## Running tests

After building, you can run tests by doing:

```
$ cd build
$ make test
```

## Depedencies

The jsondiff-c library dependes on a few libraries (managed via conan):
* [jansson](https://github.com/akheron/jansson) for json parsing / building
* [diff-match-patch-cpp-stl](https://github.com/leutloff/diff-match-patch-cpp-stl)
  for running diff-match-patch on strings
* [doctest](https://github.com/onqtam/doctest) for unit testing

Additionally, tools & examples depend on:
* [argtable3](https://github.com/argtable/argtable3) for argument parsing

