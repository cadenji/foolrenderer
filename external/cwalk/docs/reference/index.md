---
title: Reference
description: A complete reference of the cwalk path library for C/C++.
---

## Basics
These are some basic, helpful functions available in the library. The basename is the last portion of the path which determines the name of the file or folder which is being pointed to. For instance, the path ``/var/log/test.txt`` would have the basename ``test.txt``. The dirname is the opposite - the path up to the basename. In that example the dirname would be ``/var/log``.

### Functions
* **[cwk_path_get_basename]({{ site.baseurl }}{% link reference/cwk_path_get_basename.md %})**  
Gets the basename of a file path.

* **[cwk_path_change_basename]({{ site.baseurl }}{% link reference/cwk_path_change_basename.md %})**  
Changes the basename of a file path.

* **[cwk_path_get_dirname]({{ site.baseurl }}{% link reference/cwk_path_get_dirname.md %})**  
Gets the dirname of a file path.

* **[cwk_path_get_root]({{ site.baseurl }}{% link reference/cwk_path_get_root.md %})**  
Determines the root of a path.

* **[cwk_path_change_root]({{ site.baseurl }}{% link reference/cwk_path_change_root.md %})**  
Changes the root of a path.

* **[cwk_path_is_absolute]({{ site.baseurl }}{% link reference/cwk_path_is_absolute.md %})**  
Determine whether the path is absolute or not.

* **[cwk_path_is_relative]({{ site.baseurl }}{% link reference/cwk_path_is_relative.md %})**  
Determine whether the path is relative or not.

* **[cwk_path_join]({{ site.baseurl }}{% link reference/cwk_path_join.md %})**  
Joins two paths together.

* **[cwk_path_join_multiple]({{ site.baseurl }}{% link reference/cwk_path_join_multiple.md %})**  
Joins multiple paths together.

* **[cwk_path_normalize]({{ site.baseurl }}{% link reference/cwk_path_normalize.md %})**  
Creates a normalized version of the path.

* **[cwk_path_intersection]({{ site.baseurl }}{% link reference/cwk_path_intersection.md %})**  
Finds common portions in two paths.

## Navigation
One might specify paths containing relative components ``../``. These functions help to resolve or create relative paths based on a base path.

### Functions
* **[cwk_path_get_absolute]({{ site.baseurl }}{% link reference/cwk_path_get_absolute.md %})**  
Generates an absolute path based on a base.

* **[cwk_path_get_relative]({{ site.baseurl }}{% link reference/cwk_path_get_relative.md %})**  
Generates a relative path based on a base.

## Extensions
Extensions are the portion of a path which come after a `.`. For instance, the file extension of the ``/var/log/test.txt`` would be ``.txt`` - which indicates that the content is text.

### Functions
* **[cwk_path_get_extension]({{ site.baseurl }}{% link reference/cwk_path_get_extension.md %})**  
Gets the extension of a file path.

* **[cwk_path_has_extension]({{ site.baseurl }}{% link reference/cwk_path_has_extension.md %})**  
Determines whether the file path has an extension.

* **[cwk_path_change_extension]({{ site.baseurl }}{% link reference/cwk_path_change_extension.md %})**  
Changes the extension of a file path.

## Segments
A segment represents a single component of a path. For instance, on linux a path might look like this ``/var/log/``, which consists of two segments ``var`` and ``log``.

### Functions
* **[cwk_path_get_first_segment]({{ site.baseurl }}{% link reference/cwk_path_get_first_segment.md %})**  
Gets the first segment of a path.

* **[cwk_path_get_last_segment]({{ site.baseurl }}{% link reference/cwk_path_get_last_segment.md %})**  
Gets the last segment of the path.

* **[cwk_path_get_next_segment]({{ site.baseurl }}{% link reference/cwk_path_get_next_segment.md %})**  
Advances to the next segment.

* **[cwk_path_get_previous_segment]({{ site.baseurl }}{% link reference/cwk_path_get_previous_segment.md %})**  
Moves to the previous segment.

* **[cwk_path_get_segment_type]({{ site.baseurl }}{% link reference/cwk_path_get_segment_type.md %})**  
Gets the type of the submitted path segment.

* **[cwk_path_change_segment]({{ site.baseurl }}{% link reference/cwk_path_change_segment.md %})**  
Changes the content of a segment.

## Style
The path style describes how paths are generated and parsed. **cwalk** currently supports two path styles, ``CWK_STYLE_WINDOWS`` and ``CWK_STYLE_UNIX``.

### Functions
* **[cwk_path_guess_style]({{ site.baseurl }}{% link reference/cwk_path_guess_style.md %})**  
Guesses the path style.

* **[cwk_path_set_style]({{ site.baseurl }}{% link reference/cwk_path_set_style.md %})**  
Configures which path style is used.

* **[cwk_path_get_style]({{ site.baseurl }}{% link reference/cwk_path_get_style.md %})**  
Gets the path style configuration.