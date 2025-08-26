# String.h
This is an header only C library for handling Strings, to add to your project you just need to Include String.h

## Example:
```c
#define STRING_IMPLEMENTATION
#include "String.h"
#include <stdio.h>

int main(int argc, char** argv){
    String str = String_new_from_cstr("Hello, World");
    printf("%s!", STRARG(str));
    
    ....
    
    return 0;
}
```

## Tests

To run tests you just need a C compiler to compile the nob.c file

```bash
gcc -o tester bil.c
```

This file uses the [nob.h](https://github.com/tsoding/nob.h/) technology to let C act like scripts.
After compiled you can run it to test all the tests/*.c files


```bash
./tester tests
```

or you can run a specific test

```bash
./tester run app
```
