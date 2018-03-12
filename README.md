# jsondiff-c
An MIT-licensed C implementation of Simperium's [jsondiff](https://github.com/simperium/jsondiff) library

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

* jsondiff-c currently requires gcc/g++ 7. I'll fix this at a later date.
* verify this all works on Linux & Windows
* transform APIs (to apply a diff object to another diff object)
* more example programs

## Building

jsondiff-c uses the cmake build system and the conan package management system.
To build it, do the following:

```
# create build folder
$ mkdir build && cd build
# install dependencies via conan - this requires internet access
$ conan install ..
# generate build files
$ cmake ..
# build
$ make
```

You can also use conan directly:

```
$ mkdir build && cd build
$ conan install ..
$ conan build ..
``

## Running tests

After building, you can run tests by doing:

```
$ cd build
$ make test
```

