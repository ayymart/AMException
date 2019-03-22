#include <stdio.h>
#include <stdlib.h>
#include "AMException.h"

_Thread_local struct _JumpStack *_jumpStackPtr;

_Noreturn void AMExceptionThrow(int num, int line, const char *file) {
    // if there is no active try block to jump to, print a message and exit
    if (_jumpStackPtr == NULL) {
        fprintf(stderr, "AMException %s:%d: unhandled exception %d\n", file, line, num);
        exit(EXIT_FAILURE);
    }

    _jumpStackPtr->num = num;
    _jumpStackPtr->line = line;
    _jumpStackPtr->file = file;
    longjmp(_jumpStackPtr->buf, num);
}