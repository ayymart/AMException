#ifndef AMEXCEPTION_H
#define AMEXCEPTION_H


#include <setjmp.h>

#if defined(TRY) || defined(CATCH) || defined(CATCH_ALL) || defined(FINALLY) || defined(END_TRY) || defined(THROW)
    #error "AMException: One of the exception macros is already defined"
#endif

struct AMExceptionStack {
    jmp_buf buf;
    struct AMExceptionStack *prev;
};

struct AMException {
    struct AMExceptionStack *stack;

    // if nonzero, an exception is currently being processed
    // catch statement will reset this to 0
    // if !0 when END_TRY is reached, the exception is rethrown
    int num;

    int line;
    const char *file;
};

_Noreturn void AMExceptionThrow();

extern _Thread_local struct AMException AMException;

#define AMEXCEPTIONTHROW(E, LINE, FILE)                                                             \
    do {                                                                                            \
        AMException.num = E;                                                                        \
        AMException.line = LINE;                                                                    \
        AMException.file = FILE;                                                                    \
        AMExceptionThrow();                                                                         \
    } while (0)

#define THROW(E) AMEXCEPTIONTHROW(E, __LINE__, __FILE__)

#define TRY                                                                                         \
    do {                                                                                            \
        struct AMExceptionStack AMExceptionStack;   /* declare a new jump stack */                  \
        AMExceptionStack.prev = AMException.stack;  /* remember the previous jump stack */          \
        AMException.stack = &AMExceptionStack;      /* set this as the current jump stack */        \
        switch (setjmp(AMExceptionStack.buf)) {                                                     \
            /* setjmp returns 0 on first call, so case 0 represents the try block */                \
            case 0:

#define CATCH(E) _Static_assert(E != 0, "0 cannot be thrown");                                      \
                break;                                                                              \
            case E:                                                                                 \
                AMException.num = 0;

#define CATCH_ALL                                                                                   \
                break;                                                                              \
            default:                                                                                \
                AMException.num = 0;

#define FINALLY                                                                                     \
        }                                                                                           \
        {

#define END_TRY                                                                                     \
        }                                                                                           \
        if (AMException.num != 0) AMExceptionThrow();                                               \
    } while (0)


#endif
