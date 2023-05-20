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

## usage

```c
#include <rln.h>

char *str = rln("enter something: ");
```

```sh
gcc -otest test.c -lcstr -lrln
```
