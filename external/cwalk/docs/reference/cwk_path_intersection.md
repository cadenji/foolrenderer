---
title: cwk_path_get_intersection
description: Finds common portions in two paths.
---

_(since v1.0.0)_  
Finds common portions in two paths.

## Signature
```c
size_t cwk_path_get_intersection(const char *path_base, const char *path_other);
```

## Description
This function finds common portions in two paths and returns the number characters from the beginning of the base path which are equal to the other path.

## Parameters
 * **path_base**: The base path which will be compared with the other path.
 * **path_other**: The other path which will compared with the base path.

## Return Value
Returns the number of characters which are common in the base path.

## Outcomes

| Style       | Base                     | Other                    | Ret.   | Result               |
|-------------|--------------------------|--------------------------|--------|----------------------|
| ``UNIX``    | ``/test/abc/../foo/bar`` | ``/test/foo/har``        | 16     | ``/test/abc/../foo`` |
| ``UNIX``    | ``/test/foo/har``        | ``/test/abc/../foo/bar`` | 9      | ``/test/foo``        |
| ``UNIX``    | ``/test/abc.txt``        | ``test/abc.txt``         | 0      | `` ``                |
| ``UNIX``    | ``/``                    | `` ``                    | 0      | `` ``                |
| ``UNIX``    | ``/this///is/a//test``   | ``/this//is/a///file``   | 12     | ``/this///is/a``     |
| ``UNIX``    | ``/this/is/a/test``      | ``/this/is/a/``          | 10     | ``/this/is/a``       |
| ``UNIX``    | ``/this/is/a/test``      | ``/this/is/a``           | 10     | ``/this/is/a``       |
| ``UNIX``    | ``/this/is/a/test``      | ``/this/is/a/string``    | 10     | ``/this/is/a``       |
| ``WINDOWS`` | ``C:/abc/test.txt``      | ``C:/``                  | 3      | ``C:/``              |
| ``WINDOWS`` | ``C:/abc/test.txt``      | ``C:/def/test.txt``      | 3      | ``C:/``              |
| ``WINDOWS`` | ``C:/test/abc.txt``      | ``D:/test/abc.txt``      | 0      | `` ``                |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *path;
  size_t length;

  path = "/this/is/a/test";
  length = cwk_path_get_intersection(path, "/this/is/a/string") 
  printf("The common portion is: '%.*s'", length, path);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The common portion is: '/this/is/a'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |