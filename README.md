# AMException
C exception handling macros using setjmp / longjmp. Aims to behave as one would expect coming from a language with native exception support.  
Inspired by http://groups.di.unipi.it/~nids/docs/longjump_try_trow_catch.html, with improvements (bolded below).

Supports:
- Differentiating between exception types with an integer code
- **Providing  arbitrary exception data with a void pointer (like `pthread`), available as `AMException.arg`**
- **A default `CATCH_ALL` block (the exception number is available as `AMException.num`)**
- A `FINALLY` block that is always executed, even if an exception occurred

Other improvements upon the source material:
- Exceptions can come from outside the current stack frame
- An exception that is not caught by any `CATCH` clause in the current `TRY` group is rethrown
- An exception that is not caught by any `CATCH` clause in any `TRY` group terminates the program via `exit`

## Examples
Throwing and catching:
```c
#include "AMException.h"
int main() {
    TRY {
        THROW(2);
    } CATCH(1) {
        // Doesn't happen
    } CATCH(2) {
        // We caught it!
    } CATCH_ALL {
        // Doesn't happen
    } FINALLY {
        // Always happens
    } TRY_END;
}
```
Throwing from outside the current stack frame:
```c
#include "AMException.h"
void func() {
    THROW(1);
}
int main() {
    TRY {
        func();
    } CATCH(1) {
        // We caught it!
    } TRY_END;
}
```
Throwing an unhandled exception:
```c
#include "AMException.h"
int main() {
    THROW(1);
    // main.c:3: unhandled exception: 1
}
```
Providing arbitrary exception data:
```c
#include <stdio.h>
#include "AMException.h"
int main() {
    TRY {
        THROW_ARG(1, (void*)"Exception #1 was thrown!");
    } CATCH(1) {
        puts((char*)AMException.arg);
        // Exception #1 was thrown!
    } TRY_END;
}
```
A more complicated example:
```c
#include <stdio.h>
#include <stdlib.h>
#include "AMException.h"

enum EXCEPTION_CLASS {
    // 0 cannot be thrown
    MY_EXCEPTION = 1,
    OTHER_EXCEPTION
};

typedef struct {
    const char* msg;
    int numerator;
} MyException;

int checked_divide(int numerator, int denominator) {
    if (denominator == 0) {
        MyException *arg = malloc(sizeof(MyException));
        arg->msg = "Divide by zero. Numerator=%d.\n";
        arg->numerator = numerator;
        THROW_ARG(MY_EXCEPTION, (void*)arg);
    }
    return numerator / denominator;
}

int main() {
    TRY {
        checked_divide(1, 0);
    } CATCH(MY_EXCEPTION) {
        MyException *arg = (MyException*)AMException.arg;
        printf(arg->msg, arg->numerator);
        free(arg);
    } TRY_END;
}
```