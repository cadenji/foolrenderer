---
title: cwk_path_is_relative
description: Determine whether the path is relative or not.
---

_(since v1.0.0)_  
Determine whether the path is relative or not.

## Description
```c
bool cwk_path_is_relative(const char *path);
```

This function checks whether the path is a relative path or not. A path is considered to be relative if the root does not end with a separator.

## Parameters
 * **path**: The path which will be checked.

## Return Value
Returns ``true`` if the path is relative or ``false`` otherwise.

## Outcomes

| Style   | Path                     | Result    |
|---------|--------------------------|-----------|
| UNIX    | ``/test/``               | ``false`` |
| UNIX    | ``test.txt``             | ``true``  |
| UNIX    | ``C:\test.txt``          | ``true``  |
| UNIX    | ``\folder\``             | ``true``  |
| WINDOWS | ``/test.txt``            | ``false`` |
| WINDOWS | ``\test.txt``            | ``false`` |
| WINDOWS | ``C:\test.txt``          | ``false`` |
| WINDOWS | ``\\server\folder\data`` | ``false`` |
| WINDOWS | ``\\.\folder\data``      | ``false`` |
| WINDOWS | ``\\?\folder\data``      | ``false`` |
| WINDOWS | ``C:test.txt``           | ``true``  |
| WINDOWS | ``..\hello\world.txt``   | ``true``  |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(cwk_path_is_relative("my/path.txt")) {
    printf("The root is absolute.");
  } else {
    printf("The root is relative.");
  }

  return EXIT_SUCCESS;
}
```

Ouput:
```
The root is relative.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |