#include <stdio.h>
#include <stdlib.h>
#include "AMException.h"

_Thread_local struct AMException AMException;

// This function is variadic as a hack to allow "overloading",
// whereby the user can choose whether to provide `arg`.
_Noreturn void AMExceptionThrow(int num, void *arg, ...) {
    // if there is no active try block to jump to, print a message and exit
    if (AMException.stack == NULL) {
        fprintf(stderr,
                "%s:%d: unhandled exception: %d\n",
                AMException.file,
                AMException.line,
                AMException.num);
        exit(EXIT_FAILURE);
    }

    AMException.num = num;
    AMException.arg = arg;
    jmp_buf *buf = &AMException.stack->buf;
    AMException.stack = AMException.stack->prev;
    longjmp(*buf, AMException.num);
}

