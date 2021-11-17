---
title: cwk_path_is_absolute
description: Determine whether the path is absolute or not.
---

_(since v1.0.0)_  
Determine whether the path is absolute or not.

## Description
```c
bool cwk_path_is_absolute(const char *path);
```
This function checks whether the path is an absolute (fully qualified) path or not. A path is considered to be absolute if the root ends with a separator.

## Parameters
 * **path**: The path which will be checked.

## Return Value
Returns ``true`` if the path is absolute or ``false`` otherwise.

## Outcomes

| Style   | Path                     | Result    |
|---------|--------------------------|-----------|
| UNIX    | ``/test/``               | ``true``  |
| UNIX    | ``test.txt``             | ``false`` |
| UNIX    | ``C:\test.txt``          | ``false`` |
| UNIX    | ``\folder\``             | ``false`` |
| WINDOWS | ``/test.txt``            | ``true``  |
| WINDOWS | ``\test.txt``            | ``true``  |
| WINDOWS | ``C:\test.txt``          | ``true``  |
| WINDOWS | ``\\server\folder\data`` | ``true``  |
| WINDOWS | ``\\.\folder\data``      | ``true``  |
| WINDOWS | ``\\?\folder\data``      | ``true``  |
| WINDOWS | ``C:test.txt``           | ``false`` |
| WINDOWS | ``..\hello\world.txt``   | ``false`` |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(cwk_path_is_absolute("/my/path.txt")) {
    printf("The root is absolute.");
  } else {
    printf("The root is relative.");
  }

  return EXIT_SUCCESS;
}
```

Ouput:
```
The root is absolute.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |