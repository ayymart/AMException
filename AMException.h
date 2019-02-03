/*
 * This header introduces the following macros:
 *      -   AMEXCEPTION_INIT
 *      -   TRY
 *      -   CATCH
 *      -   FINALLY
 *      -   THROW
 *      -   RETHROW
 *
 * It also introduces the following identifiers:
 *      -   Exception
 *      -   struct JumpStack
 *      -   JumpStack
 *      -   JumpStackPop
 *      -   JumpStackPush
 *      -   _jumpStack
 */


#ifndef AMEXCEPTION_AMEXCEPTION_H
#define AMEXCEPTION_AMEXCEPTION_H


#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(TRY) || defined(CATCH) || defined(FINALLY) || defined(THROW) || defined(RETHROW)
    #error "AMException: One of the exception macros is already defined"
#endif

// line and file are only valid for num != 0
typedef struct {
    int num;
    int line;
    const char *file;
} Exception;

typedef struct JumpStack {
    jmp_buf buf;
    struct JumpStack *prev;
    Exception exception;
} JumpStack;

#define AMEXCEPTION_INIT _Thread_local JumpStack *_jumpStack

extern _Thread_local JumpStack *_jumpStack;  // is _Thread_local necessary here?

// TODO make this accept an out-param instead of returning a struct by value?
static inline Exception JumpStackPop(void) {
    // this function should only be called in END_TRY following a push in TRY
    assert(_jumpStack != NULL);
    Exception retval = _jumpStack->exception;
    JumpStack *temp = _jumpStack->prev;
    free(_jumpStack);
    _jumpStack = temp;
    return retval;
}

static inline void JumpStackPush(void) {
    JumpStack *temp = _jumpStack;
    _jumpStack = malloc(sizeof(JumpStack));
    _jumpStack->exception.num = 0;
    _jumpStack->prev = temp;
}

#define RETHROW(E, LINE, FILE)                                                                      \
    {                                                                                               \
        if (_jumpStack == NULL) {                                                                   \
            fprintf(stderr, "Unhandled exception %d at line %d of %s\n", E, LINE, FILE);            \
            exit(EXIT_FAILURE);                                                                     \
        }                                                                                           \
        _jumpStack->exception.num = E;                                                              \
        _jumpStack->exception.line = LINE;                                                          \
        _jumpStack->exception.file = FILE;                                                          \
        longjmp(_jumpStack->buf, E);                                                                \
    }

#define THROW(E) RETHROW (E, __LINE__, __FILE__)

#define TRY                                                                                         \
    do {                                                                                            \
        JumpStackPush();                                                                            \
        switch (setjmp(_jumpStack->buf)) {                                                          \
            case 0:                                                                                 \
            while (1) {

#define CATCH(E) _Static_assert(E != 0, "0 cannot be thrown");                                      \
                break;                                                                              \
            case E:                                                                                 \
                _jumpStack->exception.num = 0;

#define FINALLY                                                                                     \
                break;                                                                              \
            }                                                                                       \
            default: {

#define END_TRY                                                                                     \
                break;                                                                              \
            }                                                                                       \
        }                                                                                           \
        {                                                                                           \
            Exception e = JumpStackPop();                                                           \
            if (e.num != 0) RETHROW (e.num, e.line, e.file);                                        \
        }                                                                                           \
    } while (0);


#endif //AMEXCEPTION_AMEXCEPTION_H
