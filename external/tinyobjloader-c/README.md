# tinyobjloader in C

Tiny but powerful header only wavefront obj loader written in C99.

If you are looking for C++ version, please see https://github.com/syoyo/tinyobjloader

## Current status

Experimental. Loading geometry data would be OK, More testing required for materials and shapes.

## Features

* Single file, header only
* Dependency-free pure C99 implementation(except for `libc`)
* Provides mostly similar functionality as in C++ version. https://github.com/syoyo/tinyobjloader
* Moderate speed and moderate memory consumption
  * Rungholt 7M triangle scene(260 MB) can be loaded in 4.7 secs and consumes 3.6 GB memory at the maximum on MacBook12 Core m5 1.2 GHz(single core use)
* Unit tests using acutest thanks to @andystanton


## Usage

Copy `tinyobj_loader_c.h` to your project.

```
/* define TINYOBJ_LOADER_C_IMPLEMENTATION for only *one* .c */
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader_c.h"

...

```

See `examples/viewer/` for more details.

tinyobjloader allocates memory. To replace the functions used for allocation,
define `TINYOBJ_MALLOC`, `TINYOBJ_REALLOC`, `TINYOBJ_CALLOC` and `TINYOBJ_FREE` in the .c file
you defined `TINYOBJ_LOADER_C_IMPLEMENTATION` in, before including `tinyobj_loader_c.h`.
Define either all or none of them. They replace `malloc`, `realloc`, `calloc` and `free` respectively.

Example:
```
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#define TINYOBJ_MALLOC my_malloc
#define TINYOBJ_REALLOC my_realloc
#define TINYOBJ_CALLOC my_calloc
#define TINYOBJ_FREE my_free
#include "tinyobj_loader_c.h"
```

## Tests

The single header test library [acutest](https://github.com/mity/acutest) is used to provide a test runner and assertion macros. There are two test suites: one for the API and one for the internal functions. Ultimately the internal tests should be removed, but are useful while the project is volatile.

The tests can be run from the project root using:

```
$ make test
```

This builds and executes a binary called `tinyobj_tests` in the test folder. There are some options to run specific tests that can be passed to the executable directly that are described on the [acutest readme](https://github.com/mity/acutest#running-unit-tests).

By default acutest forks for each test. To disable this for debugging purposes, you can pass the switch `--no-exec` to `tinyobj_tests`.

## License

MIT license.

### Third party licenses

* acutest Another C/C++ Unit Test facility. MIT license. http://github.com/mity/acutest

## TODO

* [ ] Windows build?
