# rln (readline)

readline library for unix<br/>
depends on <a href="https://Eldyj/github.com/cstr.h">cstr.h library</a>

## binds

- left = move left
- right = move right
- home (ctrl+a) = to start
- end (ctrl+e), ctrl+d = to end
- ctrl+left = previous word
- ctrl+right = next word
- backspace (ctrl+h) = remove before cursor
- delete = remove after cursor
- ctrl+q = remove first character
- ctrl+w = remove last character
- ctrl+k = clear before cursor
- ctrl+l = clear after cursor
- ctrl+o = clear all input

## usage

```c
#include <rln.h>
//...
char *str = rln("enter something: ");
//...
free(str);
```

## examples

<a href="./examples">here</a>

