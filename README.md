# rln (readline)

readline library for unix with vim-like binds<br/>
depends on <a href="https://Eldyj/github.com/cstr.h">cstr.h library</a>

## binds

- shift+h = move left
- shift+l = move right
- shift+j = to start
- shift+k = to end
- shift+b = previous word
- shift+n = next word
- shift+u = remove before cursor
- shift+i = remove after cursor
- shift+a = remove first character
- shift+s = remove last character
- shift+z = clear before cursor
- shift+x = clear after cursor

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

