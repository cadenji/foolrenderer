---
title: cwk_path_has_extension
description: Determines whether the file path has an extension.
---

_(since v1.0.0)_  
Determines whether the file path has an extension.

## Description
```c
bool cwk_path_has_extension(const char *path);
```

This function determines whether the submitted file path has an extension. This 
will evaluate to true if the last segment of the path contains a dot. In order 
to read more information about the extension see **[cwk_path_get_extension]({{ site.baseurl }}{% link reference/cwk_path_get_extension.md %})**.

## Parameters
 * **path**: The path which will be inspected.

## Return Value
Returns ``true`` if the path has an extension or ``false`` otherwise.

## Outcomes

| Path                       | Result     |
|----------------------------|------------|
| ``/my/path.txt``           | ``true``   |
| ``/my/path``               | ``false``  |
| ``/my/.path``              | ``true``   |
| ``/my/path.``              | ``true``   |
| ``/my/path.abc.txt.tests`` | ``true``   |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (cwk_path_has_extension("/my/path.txt")) {
    printf("yes.");
  } else {
    printf("no.");
  }

  return EXIT_SUCCESS;
}
```

Ouput:
```
yes.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
