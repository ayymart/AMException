/*
 * This header introduces the following identifiers:
 *
 *  Macros:
 *      -   AMEXCEPTION_INIT
 *      -   TRY
 *      -   CATCH
 *      -   FINALLY
 *      -   THROW
 *      -   RETHROW
 *
 *  Structs:
 *      -   _JumpStack
 *
 *  Global Variables:
 *      -   _jumpStackPtr
 *
 *  Local Variables (within the TRY ... END_TRY block)
 *      -   _jumpStack
 */


#ifndef AMEXCEPTION_AMEXCEPTION_H
#define AMEXCEPTION_AMEXCEPTION_H


#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(TRY) || defined(CATCH) || defined(FINALLY) || defined(THROW) || defined(RETHROW)
    #error "AMException: One of the exception macros is already defined"
#endif

// line and file are only valid for num != 0
struct _JumpStack {
    jmp_buf buf;                // for use with setjmp and longjmp
    struct _JumpStack *prev;    // previous value of the global pointer
    int num;                    // set to the thrown value, if != 0 during END_TRY the exception is rethrown
    int line;                   // location of originating THROW
    const char *file;           // ""
};

#define AMEXCEPTION_INIT _Thread_local struct _JumpStack *_jumpStackPtr

extern _Thread_local struct _JumpStack *_jumpStackPtr;

#define RETHROW(E, LINE, FILE)                                                                      \
    {                                                                                               \
        if (_jumpStackPtr == NULL) {                                                                \
            fprintf(stderr, "Unhandled exception %d at line %d of %s\n", E, LINE, FILE);            \
            exit(EXIT_FAILURE);                                                                     \
        }                                                                                           \
        _jumpStackPtr->num = E;                                                                     \
        _jumpStackPtr->line = LINE;                                                                 \
        _jumpStackPtr->file = FILE;                                                                 \
        longjmp(_jumpStackPtr->buf, E);                                                             \
    }

#define THROW(E) RETHROW (E, __LINE__, __FILE__)

#define TRY                                                                                         \
    do {                                                                                            \
        struct _JumpStack _jumpStack;       /* declare a new jump stack */                          \
        _jumpStack.num = 0;                 /* will be set if an exception is thrown */             \
        _jumpStack.prev = _jumpStackPtr;    /* store the address of the previous jump stack */      \
        _jumpStackPtr = &_jumpStack;        /* set this as the current jump stack */                \
        switch (setjmp(_jumpStack.buf)) {                                                           \
            /* setjmp returns 0 on first call, so case 0 represents TRY { } */                      \
            case 0:                                                                                 \
            /* loop encompasses every switch case except default, */                                \
            /* allowing jumping to FINALLY using break statement */                                 \
            while (1) {

#define CATCH(E) _Static_assert(E != 0, "0 cannot be thrown");                                      \
                /* break out of TRY or previous CATCH and go to FINALLY */                          \
                break;                                                                              \
            case E:                                                                                 \
                _jumpStack.num = 0;         /* note that the exception has been handled */

#define FINALLY                                                                                     \
                break;                                                                              \
            }                                                                                       \
            default: {

#define END_TRY                                                                                     \
                break;                                                                              \
            }                                                                                       \
        }                                                                                           \
        {                                                                                           \
            /* restore the previous jump stack */                                                   \
            /* if there was an unhandled exception, rethrow it */                                   \
            _jumpStackPtr = _jumpStack.prev;                                                        \
            if (_jumpStack.num != 0) RETHROW (_jumpStack.num, _jumpStack.line, _jumpStack.file);    \
        }                                                                                           \
    } while (0);


#endif //AMEXCEPTION_AMEXCEPTION_H
