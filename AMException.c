#include <stdio.h>
#include <stdlib.h>
#include "AMException.h"

_Thread_local struct AMException AMException;

_Noreturn void AMExceptionThrow() {
    // if there is no active try block to jump to, print a message and exit
    if (AMException.stack == NULL) {
        fprintf(stderr,
                "AMException %s:%d: unhandled exception %d\n",
                AMException.file,
                AMException.line,
                AMException.num);
        exit(EXIT_FAILURE);
    }

    jmp_buf *buf = &AMException.stack->buf;
    AMException.stack = AMException.stack->prev;
    longjmp(*buf, AMException.num);
}
