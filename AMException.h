#ifndef AMEXCEPTION_H
#define AMEXCEPTION_H


#include <setjmp.h>

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

extern _Thread_local struct _JumpStack *_jumpStackPtr;

void AMExceptionThrow(int num, int line, const char *file);

#define THROW(E) AMExceptionThrow(E, __LINE__, __FILE__);

#define TRY                                                                                         \
    do {                                                                                            \
        struct _JumpStack _jumpStack;       /* declare a new jump stack */                          \
        _jumpStack.num = 0;                 /* will be set if an exception is thrown */             \
        _jumpStack.prev = _jumpStackPtr;    /* store the address of the previous jump stack */      \
        _jumpStackPtr = &_jumpStack;        /* set this as the current jump stack */                \
        switch (setjmp(_jumpStack.buf)) {                                                           \
            /* setjmp returns 0 on first call, so case 0 represents TRY { } */                      \
            case 0:

#define CATCH(E) _Static_assert(E != 0, "0 cannot be thrown");                                      \
                break;                                                                              \
            case E:                                                                                 \
                _jumpStack.num = 0;

#define CATCH_ALL                                                                                   \
                break;                                                                              \
            default:                                                                                \
                _jumpStack.num = 0;

#define FINALLY                                                                                     \
        }                                                                                           \
        {

#define END_TRY                                                                                     \
        }                                                                                           \
        _jumpStackPtr = _jumpStack.prev;                                                            \
        if (_jumpStack.num != 0) AMExceptionThrow(_jumpStack.num, _jumpStack.line, _jumpStack.file);\
    } while (0)


#endif